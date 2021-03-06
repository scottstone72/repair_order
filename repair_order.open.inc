<?php

require_once(DRUPAL_ROOT  . '/' . drupal_get_path('module','repair_order') . '/includes/class.repair_order.inc.php');
drupal_add_css(drupal_get_path('module', 'repair_order') . '/dist/css/min/repair_order_open.min.css');
drupal_add_js(drupal_get_path('module', 'repair_order') . '/dist/js/repair_open_completed.min.js', array('scope' => 'footer'));
drupal_add_js(drupal_get_path('module', 'repair_order') . '/dist/js/jquery.cookie.js');

/**
 * Show form once callback has fired.
 * @return mixed
 */
function open_repair_orders_page() {

  if(!user_access('view repair order approvals and submissions')) {
    return MENU_ACCESS_DENIED;
  }

  $build = array();
  $repairs = new RepairOrder(); // Get repair order object.
  $repairs_arr = $repairs->get_all_open_repair_items();



  $build['explain_text'] = array(
    '#type' => 'markup',
    '#markup' => "<div class='repair-message'><p>Repair Orders with a currently Open Status.
    Click the 'Show Details link to see and update each Repair Order.</p></div>",
  );


  foreach ($repairs_arr as $key => $val) {
    $build[] = drupal_get_form('open_repair_orders_form_' . $val['order_num'], $val);

    // Unserialize to prepare for js.
    // + operator is a union for arrays, preserves array keys if there are no duplicates else will override.
    //    $repair_data = $repair_data + unserialize($val['repair_data']);
  }
  return $build;
}
/**
 * Repair_Order_approval callback
 */
function open_repair_orders_form($form, &$form_state, $args) {

  global $user;

  $order = $args; // Hand over our form data

  // Create header array for table
  $header = array(
    'order_num' => t('Repair Order#'),
    'unit_id' => t('Repair Item'),
    'more_details' => t('Details'),
    'date_submitted' => t('Date Submitted'),
    'follow_up_alert' => t('Follow Up By'),
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
  $follow_up_alert = $order['follow_up_alert'];
  $submitted_by = $order['submitted_by'];
  $admin_notes = $order['admin_notes'];
  $administrator = $order['administrator'];

  $repair_order_js['order_num'] = array();

  // Get repair_order_js variable if one exists
  $repair_order_var = variable_get('repair_order_' . $order_num);

  // If variable exists than make a cookie for js to have
  if (isset($repair_order_var)) {
    $repair_order_var = unserialize($repair_order_var);
    $repair_order_js['order_num'] = $repair_order_var;
    setcookie('repair_order_' . $order_num, json_encode($repair_order_js));
  }


  // Break out into arrays.
  $repair_order_data = unserialize($order['repair_order_data']);
  $repairs_req = $repair_order_data['order'][$order_num]['repairs_req'];
  $time_req = $repair_order_data['order'][$order_num]['time_req'];
  $parts_req = $repair_order_data['order'][$order_num]['parts_req'];
  $parts_price = $repair_order_data['order'][$order_num]['parts_price'];


  // Link for modal forms
  $links['pop_form'] = '<span id="' . $order_num . '" class="show-details">Show Details</span>';
  $links['print'] = '<a href="/repair_order/open_repairs/print/'. $order_num .'">Print Preview</a>';

  $rows = array();
  // Build the rows and add data
  $rows[$order_num] = array(
    'order_num' => $order_num,
    'unit_id' => $unit_id,
    'more_details' => $links['pop_form'],
    'date_submitted' => $date_submitted,
    'follow_up_alert' => $follow_up_alert,
    'print' => $links['print'],
  );

  $form['approval_table' . $order_num] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
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
    '#type' => 'select',
    '#title' => 'Priority Level',
    '#default_value' => $repair_priority,
    //    '#description' => t('Please select a priority level for this repair.'),
    '#options' => array(
      'Low' => 'Low',
      'Medium' => 'Medium',
      'High' => 'High',
    ),
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
      '#prefix' => '<div class="repairs-req">',
      '#suffix' => '</div>',
    );

    $form['container' . $order_num]['repair_fieldset']['repairs'][$i]['time_req'][$i] = array(
      '#type' => 'textfield',
      '#title' => 'Time required',
      '#default_value' => $time_req[$i],
      '#prefix' => '<div class="time-req">',
      '#suffix' => '</div>',
    );


    $form['container' . $order_num]['repair_fieldset']['repairs'][$i]['repair_complete'][$i] = array(
      '#type' => 'button',
      '#value' => t('Completed'),
      '#default_value' => 0,
	  '#id' => $order_num . '_repair_complete_' . $i,
      '#attributes' => array('class' => array("btn btn-danger btn-sm open order_$order_num"), 'data-toggle' => 'button'),
      '#prefix' => '<div class="repair-complete-button">',
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
      '#prefix' => '<div class="parts-req">',
      '#suffix' => '</div>',
    );

    $form['container' . $order_num]['parts_fieldset']['parts'][$i]['parts_price'][$i] = array(
      '#type' => 'textfield',
      '#title' => 'Price $',
      '#default_value' => $parts_price[$i],
      '#prefix' => '<div class="parts-price">',
      '#suffix' => '</div>',
      //      '#ajax' => array(
      //        'callback' => 'local_app_ajax_change_price_field_callback',
      //        'wrapper' => 'local_app_change_price_wrapper',
      //        'effect' => 'fade',
      //'method' => 'replace',
      //      ),
    );

    $form['container' . $order_num]['parts_fieldset']['parts'][$i]['part_complete'][$i] = array(
      '#type' => 'button',
      '#value' => t('Part Installed'),
	  '#id' => $order_num . '_part_complete_' . $i,
	  '#attributes' => array('class' => array("btn-sm btn-danger open order_$order_num"), 'data-toggle' => 'button'),
      '#prefix' => '<div class="parts-complete-button">',
      '#suffix' => '</div>',
    );

//	$total_cost = $total_cost + $parts_price[$i];
  }

  $form['container' . $order_num]['footer'] = array(
    '#type' => 'fieldset',
    '#id' => 'footer-fieldset',
    '#attributes' => array('class' => array('admin-fieldset')),
  );

  $form['container' . $order_num]['footer']['total_price'] = array(
    '#type' => 'textfield',
    '#title' => 'Total Cost $',
    '#default_value' => $repair_cost/*isset($total_cost) ? number_format($total_cost, 2, '.', '') : 0*/,
    '#attributes' => array('class' => array('total-price-textfield')),
    //    '#prefix' => '<div id="local_app_change_price_wrapper">',
    //    '#suffix' => '</div>',
    '#prefix' => '<div class="total-cost">',
    '#suffix' => '</div>',

  );

  //***************************************************************************//


  $form['container' . $order_num]['footer']['follow_up_alert'] = array(
    '#type' => 'textfield',
    '#title' => 'Follow Up By',
    '#default_value' => $follow_up_alert,
    '#attributes' => array('class' => array('ui-datepicker')),
    '#prefix' => '<div class="follow-up-alert">',
    '#suffix' => '</div>',
    '#required' => TRUE,
  );
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
    '#prefix' => '<div class="admin-notes">',
    '#suffix' => '</div>',
  );

//  $form['container' . $order_num]['row_3_open']['#markup'] = '<div class="row">'; // Open row 3

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


  $form['container' . $order_num]['admin']['update'] = array(
    '#type' => 'submit',
	'#validate' => array('update_repair_order_validate'),
	'#submit' => array('update_repair_order_submit'),
    '#value' => t('Update'),
    '#prefix' => '<div class="update-button">',
    '#suffix' => '</div>',
  );

//  $form['container' . $order_num]['row_3_closed']['#markup'] = '</div>'; // Close row 3


  $form['container' . $order_num]['admin']['submit'] = array(
	'#type' => 'submit',
	'#value' => t('Repair Completed'),
	'#attributes' => array('class' => array('btn btn-primary')),
	'#prefix' => '<div class="submit-button">',
	'#suffix' => '</div>',
  );

  $form['container' . $order_num]['admin']['cancel'] = array(
    '#markup' => '<div><span id="' . $order_num . '"  class="cancel-btn-val">Hide Form</span></div>',
    '#prefix' => '<div class="cancel-button">',
    '#suffix' => '</div>',
  );

  return $form;
}

/**
 * Repair order update validate
 */
function update_repair_order_validate($form, &$form_state) {

  $order_num = $form_state['values']['submit_data']['order_num'];
  $container = 'container' . $order_num;
  // Grab cookie data to save as variable
  $repair_js = json_decode($_COOKIE['repair_data']);
  $repair_js = serialize($repair_js);
  // Add repair data to form_state array.
  $form_state['values']['submit_data']['repair_js'] = $repair_js;



  // Lets prepare our multiple form values for the submit function.
  $repairs_req = $time_req = $parts_req = $parts_price = array();
  // Lets concatenate our fields together to enter into database
  for ($i = 0; $i < count($form_state['values'][$container]['repair_fieldset']['repairs']); $i++) {
	// Add the values to array so we can hand over to implode() to convert to pipe separated string.
	$repairs_req[] = $form_state['values'][$container]['repair_fieldset']['repairs'][$i]['repairs_req'][$i];
	$time_req[] = $form_state['values'][$container]['repair_fieldset']['repairs'][$i]['time_req'][$i];
  }

  for ($i = 0; $i < count($form_state['values'][$container]['parts_fieldset']['parts']); $i++) {
	// Add the values to array so we can hand over to implode() to convert to comma separated string.
	$parts_req[] = $form_state['values'][$container]['parts_fieldset']['parts'][$i]['parts_req'][$i];
	$parts_price[] = number_format($form_state['values'][$container]['parts_fieldset']['parts'][$i]['parts_price'][$i], 0, '', ''); // Get rid of the decimals
  }

  // Organize and store our repair data
  $repair_data['order'] = array(
	$order_num => array(
	  'repairs_req' => $repairs_req,
	  'time_req' => $time_req,
	  'parts_req' => $parts_req,
	  'parts_price' => $parts_price,
	),
  );

  // Save the serialized arrays back to form_state.
  $form_state['values']['repair_order_data'] = serialize($repair_data);

}

/**
 * Repair order update submit
 */
function update_repair_order_submit($form, &$form_state) {

  $order_num = $form_state['values']['submit_data']['order_num'];
  $nid = $form_state['values']['submit_data']['nid'];
  $unit_id = $form_state['values']['submit_data']['unit_id'];
  $repair_js = $form_state['values']['submit_data']['repair_js'];
  $container = 'container' . $order_num;

  // Set any active button states to variables table
  variable_set('repair_order_' . $order_num, $repair_js);

  $query = db_update('repair_orders');
  $fields = array(
	//    'order_num' => $form_state['values']['container']['order_num'],
	'repair_priority' => $form_state['values'][$container]['repair_priority'],
	'po_num' => $form_state['values'][$container]['po_num'],
	'last_job' => $form_state['values'][$container]['last_job'],
	'repair_order_data' => $form_state['values']['repair_order_data'],
	'repair_cost' => $form_state['values'][$container]['footer']['total_price'],
	'follow_up_alert' => $form_state['values'][$container]['footer']['follow_up_alert'],
	'submitted_by' => $form_state['values'][$container]['footer']['submitted_by'],
	'approved' => 'Approved',
	'admin_notes' => $form_state['values'][$container]['admin']['notes'],
	'administrator' => $form_state['values'][$container]['admin']['admin_approver'],
  );

  $query->fields($fields)
	    ->condition('order_num', $order_num)
  	    ->execute();

  drupal_set_message(t('Repair Order #' . $order_num .  ' for equipment item ' . $unit_id . ' has been UPDATED.'));


}

/**
 * Submit the data to the database as COMPLETED.
 */
function open_repair_orders_form_submit($form, &$form_state) {

  $order_num = $form_state['values']['submit_data']['order_num'];
  $nid = $form_state['values']['submit_data']['nid'];
  $unit_id = $form_state['values']['submit_data']['unit_id'];
  $container = 'container' . $order_num;

  // Submit our data
  $query = db_update('repair_orders');
  $fields = array(
	'repair_priority' => $form_state['values'][$container]['repair_priority'],
	'po_num' => $form_state['values'][$container]['po_num'],
	'last_job' => $form_state['values'][$container]['last_job'],
	'repair_cost' => $form_state['values'][$container]['footer']['total_price'],
    'repair_order_data' => '',
    'follow_up_alert' => '',
	'submitted_by' => $form_state['values'][$container]['footer']['submitted_by'],
	'approved' =>  date('F d, Y'),
	'completed' => 'Completed',
	'admin_notes' => $form_state['values'][$container]['admin']['notes'],
	'administrator' => $form_state['values'][$container]['admin']['admin_approver'],
  );

  $query->fields($fields)
	->condition('order_num', $order_num)
	->execute();

  // Delete the variable for this order since we no longer will need it.
  variable_del('repair_order_' . $order_num);

      // Update the field_data_field_status table with new field_status_value
      db_merge('field_data_field_status')
        ->key(array(
          'entity_type' => 'node',
          'bundle' => 'rental_product',
          'entity_id' => $nid,
        ))
        ->fields(array(
          'field_status_value' => 'Open',
          'deleted' => 0,
          'revision_id' => 0,
          'language' => 'und',
          'delta' => 0,
        ))
        ->execute();

  // Clear the field_data_fields cache
  $cid = 'field:node:';
  cache_clear_all($cid, 'cache_field', TRUE);

  drupal_set_message(t('Repair Order #' . $order_num .  ' for equipment item ' . $unit_id . ' has been completed.'));
}
