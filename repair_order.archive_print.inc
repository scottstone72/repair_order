<?php

require_once(DRUPAL_ROOT  . '/' . drupal_get_path('module','repair_order') . '/includes/class.repair_order.inc.php');
drupal_add_css(drupal_get_path('module', 'repair_order') . "/dist/css/min/repair_order_print.min.css");
drupal_add_js(drupal_get_path('module', 'repair_order') . "/dist/js/repair_order.min.js");

/**
 * This page displays data from each repair order
 * submission as a print friendly page
 */
function archived_repair_orders_print($args) {
  // Check user permission
  //  if(!user_access('view repair order form')) {
  //    return MENU_ACCESS_DENIED;
  //  }

  $order_num = $args;
  // Bring in repair item object
  $repair = new RepairOrder();
  $repair_item = $repair->get_repair_item($order_num);

//  $date_submitted = $priority_level = $unit_id = $po_num =
//  $last_job = $return_date = $submitted_by = '';
//  $repairs_req = $parts_req = $time_req  = $total_cost = array();

  $order_num = $repair_item['order_num'];
  $date_submitted = $repair_item['date_submitted'];
  $priority_level = $repair_item['repair_priority'];
  $unit_id = $repair_item['unit_id'];
  $po_num = $repair_item['po_num'];
  $last_job = $repair_item['last_job'];
  $total_cost = $repair_item['repair_cost'];
  $submitted_by = $repair_item['submitted_by'];
  $repair_order_data = unserialize($repair_item['repair_archive_data']);

  // Need to break apart strings and add to array
  $repairs_req = $repair_order_data['order'][$order_num]['repairs_req'];
  $time_req = $repair_order_data['order'][$order_num]['time_req'];
  $parts_req = $repair_order_data['order'][$order_num]['parts_req'];
  $parts_price = $repair_order_data['order'][$order_num]['parts_price'];


  // Send image through the theme_image function.
  $image_path = drupal_get_path('theme', 'ponderosa') . '/images/ponderosa_logo.png';

  $variables = array(
    'path' => $image_path,
    'alt' => 'Company Logo',
    'title' => 'Company Logo',
    'attributes' => array('class' => 'company-image', 'id' => 'company-image'),
  );

  $img = theme('image', $variables);

  $output = '';
  $output .= $img;
  //$output .= '<h1 id="title-top"><strong>Ponderosa Rentals</strong></h1>';
  $output .= '<div><h2 id="title-top"><strong>Repair Order</strong></h2></div>';
  $output .= "<div id='unit-id'><strong>Unit ID:</strong>&nbsp&nbsp $unit_id</div>";
  $output .= "<div id='date'><strong>Date:</strong>&nbsp&nbsp $date_submitted</div>";
  $output .= "<div id='serial'><strong>Order # </strong>&nbsp $order_num</div>";
  $output .= "<div id='priority-level'><strong>Priority Level:</strong>&nbsp&nbsp $priority_level</div>";
  $output .= "<div id='po-num'><strong>PO #:</strong>&nbsp&nbsp $po_num</div>";
  $output .= "<div id='last-job'><strong>Last Job:</strong>&nbsp&nbsp $last_job</div>";

  // Horizontal rule for easier data seperation
  $output .= '<div class="hr-contain"><hr class="horizontal-rule"/></div>';


  // Loop through $repairs_req
  //  $output .= '<div id="repairs_req">'; // Wrap a div around each item so we can sass it.
  foreach($repairs_req as $key => $value) { //todo: Fix css on the print page after hitting submit button, some weird formatting needs to be fixed.
    $key = $key + 1;
    // Add some color seperation to our rows
    if($key % 2) {
      $output .= '<div class="odd-row">';
    }
    else {
      $output .= '<div class="even-row">';
    }
    $output .= "<div class='repairs-required'><strong><span class='repair-heading'>Repair #" . $key . "</span></strong><br/>&nbsp" . $value . "</div>";
    $output .= "<div class='time-required'><strong>Time Required:</strong>&nbsp" . array_shift($time_req) . "</div>";
    $output .= '</div>'; // Close off odd or even row
  }
  //  $output .= '</div>'; // Close off $repairs_req


  // Horizontal rule for easier data seperation
  $output .= '<div class="hr-contain"><hr class="horizontal-rule"/></div>';


  // Loop through $parts_req
  //  $output .= '<div id="parts_req">';
  foreach ($parts_req as $k => $v) {
    $k = $k + 1;
    // Add some color seperation to our rows
    if($k % 2) {
      $output .= '<div class="odd-row">';
    }
    else {
      $output .= '<div class="even-row">';
    }
    $output .= "<div class='parts-required'><strong><span class='repair-heading'>Parts #" . $k . "</span></strong><br/>&nbsp" . $v . "</div>";
    $output .= "<div class='price'><strong>Price: $</strong>&nbsp" . array_shift($parts_price) . "</div>";
    $output .= '</div>'; // Close off odd or even row
  }
  //  $output .= '</div>'; // Close off $parts_req



  // Horizontal rule for easier data seperation
  $output .= '<div class="hr-contain"><hr class="horizontal-rule"/></div>';

  $output .= "<div id='total-cost'><strong>Total Repair Cost: &nbsp&nbsp$ </strong>" . number_format($total_cost, 2,'.',',') . "</div>";
  $output .= "<div id='submitted-by'><strong>Submitted By:</strong>&nbsp&nbsp $submitted_by</div>";

  $output .= '<div id="link-container">';

  $output .= '<ul class="link-list"><li>' . l(t(' <- back to Archived Repair Orders'), 'repair_order/archived_repairs',
                                              array('attributes' => array('class' => array('repair-order-link')))) . "</li>";

  $output .= '<li>' . l(t('Home'), '',
                        array('attributes' => array('class' => array('home-link')))) . "</li>";

  $output .= '<li id="print-link"><div id="print-this-page">Print Page</div></li></ul>';


  return $output;

}
