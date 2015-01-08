
(function ($) {

  Drupal.behaviors.repair_open_form_collaspe = {
    attach: function (context, settings) {

      //$('table tr td:first-child, thead tr th:first-child').remove(); // Remove the checkbox in table.
      $('input:checkbox').parent().remove();

      $('.approval-fieldset').once().hide(); // Hide the forms on page load.

      // Toggle fieldsets when user clicks show Details or the Cancel button
      $(".show-details, .cancel-btn-val", context).click(function () {

        table_num = $(this).attr('id');
        selFieldset = 'fieldset#container-'+ table_num;

        var selectdiv = $(selFieldset);
        var elem = selectdiv.slideToggle(600);
        $('.approval-fieldset').not(elem).hide();

        // Call our add repair states if table is clicked
        if(table_num)  {
            addRepairStats(table_num);
        }
      }); // End click event

    }
  };

  Drupal.behaviors.repair_open_datepicker = {
    attach: function (context, settings) {

      // This is our jquery datepicker.
      $(".ui-datepicker", context).datepicker({
        beforeShow: function (input, inst) {
          setTimeout(function () {
            inst.dpDiv.css({
              'padding': 0,
              'margin': 0,
              'z-index': 20,
              'left': 100
              //'top': 200
            });
          }, 0);
        },
        dateFormat: "M d, yy",
        minDate: 0 // Setting minDate to zero restricts dates to only future dates.
      });
    }
  };


  Drupal.behaviors.repair_get_active_states = {
        attach: function (context, settings) {

            // If user selects a completed button
            $('.open').click(function () {

              // Set jQuery cookie encode and decode automatically
              $.cookie.json = true;

              // Get clicked elements #id
              var id = $(this).attr('id');
              var repairArr = [];

              // Id of clicked button
              var selbtnId = '#' + id;

              // Get the parent table element to only search inside
              // the table that is clicked for grabbing our repair values
              // Id of parent fieldset of clicked button
              var fieldId = $(selbtnId).closest("fieldset.approval-fieldset").attr('id');

              // Get all active elements in fieldset
              $('#' + fieldId).find('button.active').each(function (index,val) {
                repairArr.push($(val).attr('id'));
              });


              if(!$(selbtnId).hasClass('active')) {

                // Add selected btn to traversed list
                repairArr.push(id);
                //console.log(repairArr);

                  // Set cookie with updated values
                  $.cookie('repair_data', repairArr);
                  //console.log(repairArr);
                  //console.log($.cookie('repair_data'));
                }

                else {

                  repairData = $.cookie('repair_data');
                  repairArr = $.grep(repairData, function (i) {
                      return i !== id;
                  });

                  $.cookie('repair_data', repairArr);
                  //console.log(repairArr);
                  //console.log($.cookie('repair_data'));

              }
            });
        }
    };

    // Add repair states to forms, sent from php
    function addRepairStats(tableNum) {
      // To filter what cookie we need to grab
      var repair_order = 'repair_order_' + tableNum;

      // parse and break out into array
      repairStats = JSON.parse($.cookie(repair_order));
      //console.log(repairStats);


      repairs = repairStats.order_num;

      $.each(repairs, function (key, val) {
        var id = '#' + val;
        // Fieldset #id's that contain our repair stats
        var repair_field = 'fieldset#repair_field_' + tableNum;
        var part_field = 'fieldset#part_field_' + tableNum;

//        console.log(repair_field + id);

        $(repair_field + ' ' +  id).addClass('active');
        $(part_field + ' ' +  id).addClass('active');
      });
    }


})(jQuery);