<?php

/**
 * @file
 * User page callbacks for the poll module.
 */

/**
 * Callback for the 'votes' tab for polls you can see other votes on
 */
function advanced_poll_votes($node) {
  $votes_per_page = 20;
  drupal_set_title($node->title);
  $admin = user_access('bypass node access') || user_access('edit any poll content');

  //if admin allow to delete all votes
  if ( $admin ) {
    drupal_add_js(drupal_get_path('module', 'advanced_poll') . '/ajax.js');
    drupal_add_js(drupal_get_path('module', 'advanced_poll') . '/advanced_poll.js');

    //if remove_all_votes sent remove votes
    if ( isset($_POST['remove_all_votes'] ) ) {
      $data = array( 'success' => true );

      //delete all votes from database
      db_delete('advanced_poll_vote')
          ->condition('nid', $node->nid)
          ->execute();

      //clear votes count from choices
      db_update('advanced_poll_choice')
        ->expression('chvotes', '0')
        ->condition('nid', $node->nid)
        ->execute();

      //send response
      die(json_encode($data));
    }
  }

  $header[] = array('data' => t('Visitor'), 'field' => 'u.name');
  $header[] = array('data' => t('Vote'), 'field' => 'pc.chtext');
  $header[] = array('data' => t('Timestamp'), 'field' => 'pv.timestamp', 'sort' => 'desc');

  $select = db_select('advanced_poll_vote', 'pv')->extend('PagerDefault')->extend('TableSort');
  $select->join('advanced_poll_choice', 'pc', 'pv.chid = pc.chid');
  $select->join('users', 'u', 'pv.uid = u.uid');
  $queried_votes = $select
    ->addTag('translatable')
    ->fields('pv', array('chid', 'uid', 'hostname', 'timestamp', 'nid'))
    ->fields('pc', array('chtext'))
    ->fields('u', array('name'))
    ->condition('pv.nid', $node->nid)
    ->limit($votes_per_page)
    ->orderByHeader($header)
    ->execute();

  $rows = array();
  foreach ($queried_votes as $vote) {
    $rows[] = array(
      $vote->name ? theme('username', array('account' => $vote)) : check_plain($vote->hostname),
      check_plain($vote->chtext),
      format_date($vote->timestamp),
    );
  }
  $build['advanced_poll_votes_table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#prefix' => t('This table lists all the recorded votes for this poll. If anonymous users are allowed to vote, they will be identified by the IP address of the computer they used when they voted.<br /><br />').( $admin ? t('<input type="button" value="Remove all votes" id="remove-all-votes" style="display: none; float: right; margin: 0 0 10px 0;" onclick="removeAllVotes()" /><span id="remove-all-votes-loader" style="display: none; float: right; margin: 0 0 10px 0;"><img src="data:image/gif;base64,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" align="absmiddle" alt="" /> Removing votes. Please wait...</span>') : '' ),
  );
  $build['advanced_poll_votes_pager'] = array('#theme' => 'pager');
  return $build;
}

/**
 * Callback for the 'results' tab for polls you can vote on
 */
function advanced_poll_results($node) {
  drupal_set_title($node->title);
  $node->show_results = TRUE;
  return node_show($node);
}

/**
 * Menu callback to provide a simple list of all polls available.
 */
function advanced_poll_page() {
  $polls_per_page = 15;

  $count_select = db_select('node', 'n');
  $count_select->addExpression('COUNT(*)', 'expression');
  $count_select->join('advanced_poll', 'p', 'p.nid = n.nid');
  $count_select->condition('n.status', 1);

  // List all polls.
  $select = db_select('node', 'n');
  $select->join('advanced_poll', 'p', 'p.nid = n.nid');
  $select->join('advanced_poll_choice', 'c', 'c.nid = n.nid');
  $select->addExpression('SUM(c.chvotes)', 'votes');
  $select = $select->fields('n', array('nid', 'title', 'created'))
    ->fields('p', array('active'))
    ->condition('n.status', 1)
    ->orderBy('n.created', 'DESC')
    ->groupBy('n.nid')
    ->groupBy('n.title')
    ->groupBy('p.active')
    ->groupBy('n.created')
    ->extend('PagerDefault')
    ->limit($polls_per_page)
    ->addTag('node_access');
  $select->setCountQuery($count_select);
  $queried_nodes = $select->execute()
    ->fetchAllAssoc('nid');

  $output = '<ul>';
  foreach ($queried_nodes as $node) {
    $output .= '<li>' . l($node->title, "node/$node->nid") . ' - ' . format_plural($node->votes, '1 vote', '@count votes') . ' - ' . ($node->active ? t('open') : t('closed')) . '</li>';
  }
  $output .= '</ul>';
  $output .= theme('pager');
  return $output;
}
