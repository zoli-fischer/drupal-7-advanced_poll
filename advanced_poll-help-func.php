<?php 

/**
 * @file
 * Helper functions for advanced poll module
 */

/**
* Create timestamp from date
*/
function date2time( $date = '' ) {
	$parts = explode('-', $date);
	return count($parts) == 3 ? mktime(0,0,0,intval($parts[1]),intval($parts[2]),intval($parts[0])) : false;
}

/**
* Create date array from date
*/
function date2array( $date = '' ) {
	if ( is_array($date) ) return $date;
	if ( $date === '' ) $date = date('Y-n-j');
	$parts = explode('-', $date);
	return count($parts) == 3 ? array(
		'day' => intval($parts[2]),
		'month' => intval($parts[1]),
		'year' => intval($parts[0])
	) : false;
}

/**
* Create date array from date
*/
function array2date( $array = array() ) {
  if ( count($array) == 0 ) $array = array(
  	'day' => date('d'),
  	'month' => date('m'),
  	'year' => date('Y')
  );
  return $array['year'].'-'.( $array['month'] < 10 ? '0' : '' ).$array['month'].'-'.( $array['day'] < 10 ? '0' : '' ).$array['day'];
}

/**
* Get servers timestamp in microtime
*
* @return double
*/
function micro_time() {
	$m = explode (' ',microtime()); 
	return(double) $m[0] + $m[1]; 
}

/**
* Check if poll need to be open or close based on open/close date 
*/
function check_poll_date( $node ) {
	$today = date('Y-m-d');
	$active = isset($node->active) ? $node->active : $node['active'];
	$use_open_date = isset($node->use_open_date) ? $node->use_open_date : $node['use_open_date'];
	$use_close_date = isset($node->use_close_date) ? $node->use_close_date : $node['use_close_date'];
	$open_date_original =  isset($node->open_date) ? array2date($node->open_date) : $node['open_date'];
	$close_date_original = isset($node->close_date) ? array2date($node->close_date) : $node['close_date'];
	
	//check if open/close dates are used for the poll
	if ( $use_open_date == 1 || $use_close_date == 1 ) {
		$open_date = $use_open_date == 1 ? $open_date_original : '0000-00-00';
		$close_date = $use_close_date == 1 ? $close_date_original : '9999-99-99';

		//echo $open_date.' <= '.$today.' && '.$close_date.' >= '.$today;
		//die();

		$active = $open_date <= $today && $close_date >= $today ? 1 : 0;
	};

	return $active;	
}


?>