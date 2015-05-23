<?php
/**
 * @file
 * Default theme implementation to display the poll results in a block.
 *
 * Variables available:
 * - $title: The title of the poll.
 * - $results: The results of the poll.
 * - $votes: The total results in the poll.
 * - $links: Links in the poll.
 * - $nid: The nid of the poll
 * - $cancel_form: A form to cancel the user's vote, if allowed.
 * - $raw_links: The raw array of links. Should be run through theme('links')
 *   if used.
 * - $vote: The choice number of the current user's vote.
 *
 * @see template_preprocess_advanced_poll_results()
 */
?>

<div class="advanced_poll">
	<div class="title">
		<?php print $title ?>
	</div>
	<?php
	if ( $show_result == false ) {
	?>
		<div class="result-date">
			<?php
				print $show_date == false
					  ?
				 	  	'The poll result will be announced after the poll is closed.'
				 	  :
				 	  	t('The poll result will be announced on %date.', array('%date' => format_date(date2time($show_date))));
			?>
		</div>
		<?php if ( $vote > 0 ) { ?>
			<div class="your-vote">
				Your vote:
				<b><?php echo $vote_title; ?></b>
			</div>
		<?php } ?>
	<?php
	} else {
	?>
		<?php print $results ?>
		<div class="total">
		<?php print t('Total votes: @votes', array('@votes' => $votes)); ?>
		</div>
	<?php
	}
	?>
</div>
<div class="links">
	<?php print $links; ?>
</div>
