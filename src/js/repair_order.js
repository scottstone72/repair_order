/**
 * This is a date_picker jquery plugin
 */
(function ($) {
    Drupal.behaviors.repair_order_print = {
        attach: function(context,settings) {

            // When user clicks Print on the print
            // pages we open the a printer window
            $("#print-this-page").click(function() {

                $('div#marble-top').css({
                    'box-shadow': 'none'
                });

                $('div.alert-block').remove();

                // Now wait for sass to change on page before loading and sending to printer
                setTimeout(function(){
                    // Now we can print page
                    window.print();
                }, 400);
            });
//            // This is our jquery datepicker.
//            $("#follow-up-alert").datepicker({dateFormat: "M d, yy"});

        }
    }


})(jQuery);
