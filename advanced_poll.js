jQuery(document).ready(function(){
	
	//handle open and close date change
	if ( jQuery('.form-item-open-date').length > 0 ) {
		updateOpenDate();
		jQuery('[name=use_open_date]').bind('change',function(){ updateOpenDate(); });
	};	
	if ( jQuery('.form-item-close-date').length > 0 ) {
		updateCloseDate();
		jQuery('[name=use_close_date]').bind('change',function(){ updateCloseDate(); });
	};
 
	//if votes in the votes table show remove votes button
	if ( jQuery('.tableheader-processed tr').length > 1 )
		jQuery('#remove-all-votes').css('display','block');

});

//handle open date change
function updateOpenDate() {
	jQuery('.form-item-open-date').css( 'display', jQuery('#edit-use-open-date-0')[0].checked ? 'none' : 'block' );
};

//handle close date change
function updateCloseDate() {
	jQuery('.form-item-close-date').css( 'display', jQuery('#edit-use-close-date-0')[0].checked ? 'none' : 'block' );
};

//remove all votes on votes pages
function removeAllVotes() {
	if ( confirm("Are you sure you want to remove all votes?\nImportant: This action can NOT be undone.") ) {
		//on ajax call error
		function error() {
			//hide show loader and show button
			jQuery('#remove-all-votes').css('display','block');
			jQuery('#remove-all-votes-loader').css('display','none');

			//show message
			alert('There was an error. Please try again.');
		};

		function success() {
			//hide loader
			jQuery('#remove-all-votes-loader').css('display','none');
			
			//remove rows
			jQuery('.tableheader-processed tbody tr').remove();      			  	
			
			//show message
			alert('All votes are removed.');
		};

		//hide button and show loader
		jQuery('#remove-all-votes').css('display','none');
		jQuery('#remove-all-votes-loader').css('display','block');
		
		ajax.post(window.location, { remove_all_votes: 1 }, function( response ) {
			var data = jQuery.parseJSON( response ); 
			if ( data && data.success === true ) {
				success();			
			} else {
				error();				    
			}
		});

		//call ajax
		/*
		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: { 'remove_all_votes': 1 },
			dataType: "json",
			success: function (data) {
				
			  alert(data);

			  if ( data && data.success === true ) {
				success();			
			  } else {
				error();				    
			  }
			},
			error: function (jqXHR, status, error) {			  
			  error();
			}
		});
		*/
	};
};