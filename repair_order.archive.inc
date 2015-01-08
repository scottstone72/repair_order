<?php
require_once(DRUPAL_ROOT  . '/' . drupal_get_path('module','repair_order') . '/includes/class.repair_order.inc.php');
drupal_add_css(drupal_get_path('module', 'repair_order') . '/dist/css/min/repair_order_archive.min.css');
drupal_add_js(drupal_get_path('module', 'repair_order') . '/dist/js/repair_open_completed.min.js');
drupal_add_js(drupal_get_path('module', 'repair_order') . '/dist/js/jquery.cookie.js');


/**
 * Show form once callback has fired.
 * @return mixed
 */
function archived_repair_orders_page() {
  if(!user_access('view repair order approvals and submissions')) {
    return MENU_ACCESS_DENIED;
  }

  return drupal_get_form('archived_repair_orders_main_form');
}

// This is our Main outer form that holds our filter and the children forms
function archived_repair_orders_main_form($form, &$form_state) {

  $form = array();
  $repairs = new RepairOrder(); // Get repair order object.
  $all_repairs = $repairs->get_all_repair_order_history();


  $form['explain_text'] = array(
    '#type' => 'markup',
    '#markup' => "<div class='repair-message'><p>Make a selection
      to bring up a table with the repair history for the Unit ID then click <strong>View Details</strong> to see each order.</p></div>",
  );

  // Get our values for our Unit Id select dropdown
  $unit_id = array();
  foreach ($all_repairs as $key => $val) {
    $unit_id[$val['unit_id']] = $val['unit_id'];
  }

  $form['filter_box'] = array(
    '#type' => 'container',
    '#attributes' => array('id' => 'filter-box'),
  );

  $form['filter_box']['unit_id_select'] = array(
    '#type' => 'select',
    '#title' => 'Filter Archive List',
    '#empty_option' => t('- Show All Repair Items -'),
    '#options' => $unit_id,
    '#ajax' => array(
      'callback' => 'get_repair_history_callback',
      'wrapper' => 'unit_select_wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );


  // Show all repairs that are completed as a default
  $unit_id = !empty($form_state['values']['unit_id_select']) ? $form_state['values']['unit_id_select'] : NULL;

  $output = array(); // Array to hold our forms
  if(!isset($unit_id)) {
    // Get all completed forms if Unit Id is not set
    $all_repairs = $repairs->get_all_repair_order_history();
    foreach ($all_repairs as $key => $val) {
      $output[] = drupal_get_form('archived_repair_orders_form_' . $val['order_num'], $val);
    }
  } else {
    // Filter forms to what user selects form select dropdown
    $all_repairs = $repairs->get_repair_order_history($unit_id);
    foreach ($all_repairs as $key => $val) {
      $output[] = drupal_get_form('archived_repair_orders_form_' . $val['order_num'], $val);
    }
  }

  $form['main_box'] = array(
    '#type' => 'container',
    '#children' => drupal_render_children($output),
    '#id' => 'unit_select_wrapper',
  );

  return $form;
}


/**
 * Ajax callback for unit id select dropdown.
 */
function get_repair_history_callback($form, &$form_state) {
  return $form['main_box'];
}


/**
 * Repair_Order_approval callback
 */
function archived_repair_orders_form($form, &$form_state, $args) {

  global $user;

  $order = $args; // Hand over our form data

  // Create header array for table
  $header = array(
    'order_num' => t('Repair Order#'),
    'unit_id' => t('Repair Item'),
    'completed' => t('Date Completed'),
    'more_details' => t('Details'),
    'date_submitted' => t('Date Submitted'),
//    'follow_up_alert' => t('Follow Up By'),
    'print' => t('Print a Copy'),
  );

  $order_num = $order['order_num'];
  $unit_id = $order['unit_id'];
  $nid = $order['nid'];
  $date_submitted = $order['date_submitted'];
  $repair_priority = $order['repair_priority'];
  $po_num = $order['po_num'];
  $last_job = $order['last_job'];
  $repair_cost = $order['repair_cost'];
  $submitted_by = $order['submitted_by'];
  $admin_notes = $order['admin_notes'];
  $administrator = $order['administrator'];
  $date_completed = $order['approved']; // Saving data in this table cause


  // Break out into arrays.
  $repair_order_data = unserialize($order['repair_archive_data']);
  $repairs_req = $repair_order_data['order'][$order_num]['repairs_req'];
  $time_req = $repair_order_data['order'][$order_num]['time_req'];
  $parts_req = $repair_order_data['order'][$order_num]['parts_req'];
  $parts_price = $repair_order_data['order'][$order_num]['parts_price'];


  // Link for modal forms
  $links['pop_form'] = '<span id="' . $order_num . '" class="show-details">Show Details</span>';
  $links['print'] = '<a href="/repair_order/archived_repairs/print/'. $order_num . '">Print Preview</a>';

  $t_rows = array();
  // Build the rows and add data
  $t_rows[$order_num] = array(
    'order_num' => $order_num,
    'unit_id' => $unit_id,
    'more_details' => $links['pop_form'],
    'date_submitted' => $date_submitted,
    'completed' => $date_completed,
    'print' => $links['print'],
  );


  $form['approval_table' . $order_num] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $t_rows,
    '#js_select' => FALSE,
    '#empty' => t('No users found'),
    '#id' => 'table_' . $order_num,
    '#multiple' => TRUE,
  );

  $form['container' . $order_num] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#id' => 'container-' .$order_num,
    '#attributes' => array('class' => array('approval-fieldset')),
  );

  //  $form['container' . $order_num]['row_1_open']['#markup'] = '<div class="row">'; // Open row 1

  $form['container' . $order_num]['order_num'] = array(
    '#type' => 'textfield',
    '#title' => 'Order #',
    '#value' => $order_num,
    '#size' => 10,
    '#disabled' => TRUE,
    '#prefix' => '<div class="order-num">',
    '#suffix' => '</div>',
  );

  //  # Date field
  $form['container' . $order_num]['date_submitted'] = array(
    '#type' => 'textfield',
    '#title' => 'Date',
    '#value' => $date_submitted,
    '#size' => 25,
    '#disabled' => TRUE,
    '#prefix' => '<div class="date-submitted">',
    '#suffix' => '</div>',
  );
  $form['container' . $order_num]['unit_id'] = array(
    '#type' => 'textfield',
    '#title' => 'Unit ID',
    '#value' => $unit_id,
    '#size' => 11,
    '#disabled' => TRUE,
    '#prefix' => '<div class="unit-id">',
    '#suffix' => '</div>',
  );
  $form['container' . $order_num]['repair_priority'] = array(
    '#type' => 'textfield',
    '#title' => 'Priority Level',
    '#default_value' => $repair_priority,
    '#disabled' => TRUE,
    //    '#description' => t('Please select a priority level for this repair.'),
    '#prefix' => '<div class="priority-level">',
    '#suffix' => '</div>',
  );
  $form['container' . $order_num]['po_num'] = array(
    '#type' => 'textfield',
    '#title' => 'PO#',
    '#size' => 25,
    '#default_value' => $po_num,
    '#prefix' => '<div class="po-num">',
    '#suffix' => '</div>',
    '#disabled' => TRUE,
  );

  $form['container' . $order_num]['last_job'] = array(
    '#type' => 'textfield',
    '#title' => 'Last job',
    '#attributes' => array('class' => array('lastjob-textarea')),
    '#size' => 60,
    '#default_value' => $last_job,
    '#disabled' => TRUE,
    '#prefix' => '<div class="last-job">',
    '#suffix' => '</div>',
  );

  //  $form['container' . $order_num]['row_1_closed']['#markup'] = '</div>'; // Close row 1

  //***************************************************************************//


  // Fieldset to hold all repair fields
  $form['container' . $order_num]['repair_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => 'Repairs Required',
    '#id' => 'repair_field_' . $order_num,
  );

  for ($i = 0; $i < count($repairs_req); $i++) {

    $form['container' . $order_num]['repair_fieldset']['repairs'][$i] = array(
      '#type' => 'container',
    );

    $form['container' . $order_num]['repair_fieldset']['repairs'][$i]['repairs_req'][$i] = array(
      '#type' => 'textarea',
      '#title' => 'Repair #' . ($i + 1),
      '#attributes' => array('rows' => '2'),
      '#default_value' => $repairs_req[$i],
      '#disabled' => TRUE,
      '#prefix' => '<div class="repairs-req">',
      '#suffix' => '</div>',
    );

    $form['container' . $order_num]['repair_fieldset']['repairs'][$i]['time_req'][$i] = array(
      '#type' => 'textfield',
      '#title' => 'Time required',
      '#default_value' => $time_req[$i],
      '#disabled' => TRUE,
      '#prefix' => '<div class="time-req">',
      '#suffix' => '</div>',
    );

  }


  //***************************************************************************//

  // Fieldset to hold all parts fields
  $form['container' . $order_num]['parts_fieldset'] = array(
    '#type' => 'fieldset',
    '#title' => 'Parts Required',
    '#id' => 'part_field_' . $order_num,
    '#prefix' => '<div id="ajax-parts-container">',
    '#suffix' => '</div>',
  );

  for ($i = 0; $i < count($parts_req); $i++) {

    $form['container' . $order_num]['parts_fieldset']['parts'][$i] = array(
      '#type' => 'container',
    );

    $form['container' . $order_num]['parts_fieldset']['parts'][$i]['parts_req'][$i] = array(
      '#type' => 'textarea',
      '#title' => 'Part #' . ($i + 1),
      '#attributes' => array('rows' => '2'),
      '#default_value' => $parts_req[$i],
      '#disabled' => TRUE,
      '#prefix' => '<div class="parts-req">',
      '#suffix' => '</div>',
    );

    $form['container' . $order_num]['parts_fieldset']['parts'][$i]['parts_price'][$i] = array(
      '#type' => 'textfield',
      '#title' => 'Price $',
      '#default_value' => $parts_price[$i],
      '#disabled' => TRUE,
      '#prefix' => '<div class="parts-price">',
      '#suffix' => '</div>',
    );

  }

  $form['container' . $order_num]['footer'] = array(
    '#type' => 'fieldset',
    '#id' => 'footer-fieldset',
  );

  $form['container' . $order_num]['footer']['total_price'] = array(
    '#type' => 'textfield',
    '#title' => 'Total Cost $',
    '#default_value' => $repair_cost,
    '#disabled' => TRUE,
    '#attributes' => array('class' => array('total-price-textfield')),
    '#prefix' => '<div class="total-cost">',
    '#suffix' => '</div>',

  );

  //***************************************************************************//

  $form['container' . $order_num]['footer']['submitted_by'] = array(
    '#type' => 'textfield',
    '#title' => 'Submitted by',
    '#value' => $submitted_by,
    '#disabled' => TRUE,
    '#prefix' => '<div class="submitted-by">',
    '#suffix' => '</div>',
  );


  // Build drop down fieldset to hold input elements
  $form['container' . $order_num]['admin'] = array(
    '#type' => 'fieldset',
    '#title' => 'Admin Section',
    '#id' => 'admin-fieldset',
    '#attributes' => array('class' => array('admin-fieldset')),
  );
  $form['container' . $order_num]['admin']['notes'] = array(
    '#type' => 'textarea',
    '#title' => t('Notes:'),
    '#default_value' => $admin_notes,
    '#disabled' => TRUE,
    '#prefix' => '<div class="admin-notes">',
    '#suffix' => '</div>',
  );

  $form['container' . $order_num]['row_3_open']['#markup'] = '<div class="row">'; // Open row 3

  $form['container' . $order_num]['admin']['admin_approver'] = array(
    '#type' => 'textfield',
    '#title' => t('Administrator name:'),
    '#value' => $administrator,
    '#disabled' => TRUE,
    '#prefix' => '<div class="admin-approver">',
    '#suffix' => '</div>',
  );

  $repair_data = array(
    'order_num' => $order_num,
    'nid' => $nid,
    'unit_id' => $unit_id,
  );

  $form['submit_data'] = array(
    '#type' => 'value',
    '#value' => $repair_data,
  );

  $form['container' . $order_num]['admin']['cancel'] = array(
    '#markup' => '<div><span id="' . $order_num . '"  class="cancel-btn-val">Close</span></div>',
    '#prefix' => '<div class="cancel-button">',
    '#suffix' => '</div>',
  );

  $form['container' . $order_num]['row_3_closed']['#markup'] = '</div>'; // Close row 3


  return $form;
}
