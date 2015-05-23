<?php

/**
 * @file
 * Builds placeholder replacement tokens for values specific to Poll nodes.
 */

/**
 * Implements hook_token_info().
 */
function advanced_poll_token_info() {
  $node['advanced_poll-votes'] = array(
    'name' => t("Poll votes"),
    'description' => t("The number of votes that have been cast on a poll."),
  );
  $node['advanced_poll-winner'] = array(
    'name' => t("Poll winner"),
    'description' => t("The winning poll answer."),
  );
  $node['advanced_poll-winner-votes'] = array(
    'name' => t("Poll winner votes"),
    'description' => t("The number of votes received by the winning poll answer."),
  );
  $node['advanced_poll-winner-percent'] = array(
    'name' => t("Poll winner percent"),
    'description' => t("The percentage of votes received by the winning poll answer."),
  );
  $node['advanced_poll-duration'] = array(
    'name' => t("Poll duration"),
    'description' => t("The length of time the poll is set to run."),
  );

  return array(
    'tokens' => array('node' => $node),
  );
}

/**
 * Implements hook_tokens().
 */
function advanced_poll_tokens($type, $tokens, array $data = array(), array $options = array()) {
  $sanitize = !empty($options['sanitize']);
  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }

  $replacements = array();

  if ($type == 'node' && !empty($data['node']) && $data['node']->type == 'advanced_poll') {
    $node = $data['node'];

    $total_votes = 0;
    $highest_votes = 0;
    foreach ($node->choice as $choice) {
      if ($choice['chvotes'] > $highest_votes) {
        $winner = $choice;
        $highest_votes = $choice['chvotes'];
      }
      $total_votes = $total_votes + $choice['chvotes'];
    }
    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'advanced_poll-votes':
          $replacements[$original] = $total_votes;
          break;

        case 'advanced_poll-winner':
          if (isset($winner)) {
            $replacements[$original] = $sanitize ? filter_xss($winner['chtext']) : $winner['chtext'];
          }
          else {
            $replacements[$original] = '';
          }
          break;

        case 'advanced_poll-winner-votes':
          if (isset($winner)) {
            $replacements[$original] = $winner['chvotes'];
          }
          else {
            $replacements[$original] = '';
          }
          break;

        case 'advanced_poll-winner-percent':
          if (isset($winner)) {
            $percent = ($winner['chvotes'] / $total_votes) * 100;
            $replacements[$original] = number_format($percent, 0);
          }
          else {
            $replacements[$original] = '';
          }
          break;

        case 'advanced_poll-duration':
          $replacements[$original] = format_interval($node->runtime, 1, $language_code);
          break;
      }
    }
  }

  return $replacements;
}
