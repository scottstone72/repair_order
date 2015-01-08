
(function ($) {

    Drupal.behaviors.repair_approval_form_collaspe = {
        attach: function (context, settings) {

            $('table tr td:first-child, thead tr th:first-child').remove(); // Remove the checkbox in table.

          $('.approval-fieldset').once().hide(); // Hide the forms on page load.

            // Toggle fieldsets when user clicks show Details or the Cancel button
            $(".show-details, .cancel-btn-val", context).click(function () {

                table_num = $(this).attr('value');
                selFieldset = 'fieldset#edit-container'+ table_num;

                var selectdiv = $(selFieldset);
                var elem = selectdiv.slideToggle(600);
                $('.approval-fieldset').not(elem).hide();

                //if(table_num)  {
                //    addRepairStats(table_num);
                //}
            }); // End click event

        }
    };

    Drupal.behaviors.repair_approval_datepicker = {
        attach: function (context, settings) {

          // This is our jquery datepicker.
            $(".ui-datepicker", context).datepicker({
                beforeShow: function (input, inst) {
                    setTimeout(function () {
                        inst.dpDiv.css({
                            'padding-top': 0,
                            'margin-top': 0,
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


})(jQuery);