<?php

drupal_add_css(drupal_get_path('module', 'repair_order') . "/dist/css/min/repair_order_print.min.css");
drupal_add_js(drupal_get_path('module', 'repair_order') . "/dist/js/repair_order.min.js");

/**
 * This page displays data from each repair order
 * submission as a print friendly page
 */
function repair_order_print_page() {
  // Check user permission
  global $user;
  if(!user_access('view repair order form')) {
    return MENU_ACCESS_DENIED;
  }


  $order_num = $_SESSION['repair_data']['order_num'];
  $date_submitted = $_SESSION['repair_data']['date_submitted'];
  $priority_level = $_SESSION['repair_data']['repair_priority'];
  $unit_id = $_SESSION['repair_data']['unit_id'];
  $po_num = $_SESSION['repair_data']['po_num'];
  $last_job = $_SESSION['repair_data']['last_job'];
  $total_cost = $_SESSION['repair_data']['repair_cost'];
  $return_date = $_SESSION['repair_data']['follow_up_alert'];
  $submitted_by = $_SESSION['repair_data']['submitted_by'];

  // Need to break apart strings and add to array
  $repair_order_data = unserialize($_SESSION['repair_data']['repair_order_data']);

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


  // Horizontal rule for easier data seperation
  $output .= '<div class="hr-contain"><hr class="horizontal-rule"/></div>';


  // Loop through $parts_req
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



  // Horizontal rule for easier data seperation
  $output .= '<div class="hr-contain"><hr class="horizontal-rule"/></div>';

  $output .= "<div id='total-cost'><strong>Total Repair Cost: &nbsp&nbsp$ </strong>" . number_format($total_cost, 2,'.',',') . "</div>";
  $output .= "<div id='follow-up-alert'><strong>Follow Up By:</strong>&nbsp&nbsp $return_date</div>";
  $output .= "<div id='submitted-by'><strong>Submitted By:</strong>&nbsp&nbsp $submitted_by</div>";

  $output .= '<div id="link-container">';

  $output .= '<ul class="link-list"><li>' . l(t(' <- back to Repair Order Form'), 'repair_order',
      array('attributes' => array('class' => array('repair-order-link')))) . "</li>";

  $output .= '<li>' . l(t('Home'), '',
      array('attributes' => array('class' => array('home-link')))) . "</li>";

  $output .= '<li id="print-link"><div id="print-this-page">Print Page</div></li></ul>';

  $output .= '</div>';

  // Clear the session
//  unset($_SESSION['repair_order_data']);
  return $output;

}
