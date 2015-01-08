<?php
require_once(DRUPAL_ROOT . '/' . drupal_get_path('module', 'repair_order') . '/includes/class.repair_order.inc.php');
drupal_add_js(drupal_get_path('module', 'repair_order') . '/dist/js/repair_order.min.js');
drupal_add_css(drupal_get_path('module', 'repair_order') . '/dist/css/min/repair_order_form.min.css');

/**
 * Show form once callback has fired.
 * @return mixed
 */
function repair_order_page() {

  if (!user_access('view repair order form')) {
	return MENU_ACCESS_DENIED;
  }

  return drupal_get_form('repair_order_form');

}

function repair_order_form($form, &$form_state) {

  global $user;
  $user_name = $user->name;
  $repair_class = new RepairOrder();
  //  $last_job = $repair_class->get_last_job();

  // Need to add library for jQuery UI datepicker widget
  drupal_add_library('system', 'ui.datepicker');


  $form['instructions'] = array(
	'#markup' => "<p><strong id='instructions'>Fields</strong> marked with a
      <strong><span class='asterick'>red</span> asterick ' <span class='asterick'>*</span> '</strong> are required.</p>",
  );

  $form['#tree'] = TRUE;

  $form['container'] = array(
	'#type' => 'fieldset',
	'#title' => 'Order Info',
  );

  // Get repair order number
  $order_num = $repair_class->repair_order_num();

  $form['start_row_1'] = array('#markup' => '<div class="row" id="row-1">'); // Start row_1

  $form['container']['order_num'] = array(
	'#type' => 'textfield',
	'#title' => 'Order #',
	'#value' => $order_num,
	'#size' => 11,
	'#disabled' => TRUE,
	'#id' => 'order-num',
	'#prefix' => '<div class="order-num">',
	'#suffix' => '</div>',
  );

  # Date field
  $form['container']['date_submitted'] = array(
	'#type' => 'textfield',
	'#title' => 'Date Submitted',
	'#value' => date('M d, Y'),
	'#size' => 25,
	'#disabled' => TRUE,
	'#prefix' => '<div class="date-submitted">',
	'#suffix' => '</div>',
  );

  $form['container']['po_num'] = array(
	'#type' => 'textfield',
	'#title' => 'PO#',
	'#size' => 25,
	'#default_value' => '',
	'#description' => t('Select a PO#.'),
	'#prefix' => '<div class="po-num">',
	'#suffix' => '</div>',
  );

  $form['end_row_1'] = array('#markup' => '</div>'); // End row_1
  $form['start_row_2'] = array('#markup' => '<div class="row" id="row-2">'); // Start row_2

  // Get the unit ID
  $unit = $repair_class->unit_id_value();

  $form['container']['unit_id'] = array(
	'#type' => 'select',
	'#title' => 'Unit ID',
//	'#description' => t('Select a Unit ID.'),
    '#empty_option' => t('- Select Unit Id -'),
//	'#required' => TRUE,
	'#options' => $unit,
	'#ajax' => array(
	  'callback' => 'get_last_job_ajax_callback',
	  'wrapper' => 'replace_last_job_wrapper',
	  'method' => 'replace',
	  'effect' => 'fade',
	),
	'#prefix' => '<div class="unit-id">',
	'#suffix' => '</div>',
	'#required' => TRUE,
  );
  $form['container']['repair_priority'] = array(
	'#type' => 'select',
	'#title' => 'Priority Level',
    '#empty_option' => t('- Select Priority Level -'),
    '#description' => t('Select a Priority Level for this repair.'),
	'#options' => array(
	  'Low' => 'Low',
	  'Medium' => 'Medium',
	  'High' => 'High',
	),
	'#required' => TRUE,
	'#prefix' => '<div class="repair-priority">',
	'#suffix' => '</div>',
  );

  $unit_id = !empty($form_state['values']['container']['unit_id']) ? $form_state['values']['container']['unit_id'] : '';
  $last_job = $repair_class->get_last_job($unit_id);

  $form['container']['last_job'] = array(
	'#type' => 'textfield',
	'#title' => 'Last job',
	'#attributes' => array('class' => array('lastjob-textarea')),
	'#size' => 60,
	'#prefix' => '<div id="replace_last_job_wrapper" class="last-job">',
	'#suffix' => '</div>',
	'#default_value' => $last_job,
	'#disabled' => TRUE,
  );

  $form['end_row_2'] = array('#markup' => '</div>'); // End row_2


  $form['start_row_5'] = array('#markup' => '<div class="row" id="row-5">'); // Start row_5

  $form['follow_up_alert'] = array(
	'#type' => 'select',
	'#title' => t('Follow Up By'),
	'#description' => t('Select a future date to be reminded and sent an emailed alert to re-check the order status.'),
	'#empty_option' => t('- Select Alert Time -'),
	'#required' => TRUE,
	'#options' => array(
	  '14 days' => t('15 Days'),
	  '30 days' => t('30 Days'),
	  '45 days' => t('45 Days'),
	  '60 days' => t('60 Days'),
	  '90 days' => t('90 Days'),
	),
	//    '#attributes' => array('class' => array('follow-up-alert')),
	'#id' => 'follow-up-alert',
	'#prefix' => '<div class="follow-up-alert">',
	'#suffix' => '</div>',
  );

  $form['submitted_by'] = array(
	'#type' => 'textfield',
	'#title' => 'Submitted by',
	'#value' => $user_name,
	'#disabled' => TRUE,
	'#prefix' => '<div class="submitted-by">',
	'#suffix' => '</div>',
  );

  $form['end_row_5'] = array('#markup' => '</div>'); // End row_5

  // Fieldset to hold all repair fields
  $form['repair_fieldset'] = array(
	'#type' => 'fieldset',
	'#title' => 'Repairs Required',
	'#prefix' => '<div id="ajax-repair-container">',
	'#suffix' => '</div>',
  );


  // $form_state['repairs_req'] to determine the number of textfields to build.
  if (empty($form_state['repair_fieldset']['repairs_req'])) {
	$form_state['repair_fieldset']['repairs_req'] = 1;
  }

  for ($i = 0; $i < $form_state['repair_fieldset']['repairs_req']; $i++) {

	$form['repair_fieldset']['repair'][$i]['repairs_req'][$i] = array(
	  '#type' => 'textarea',
	  '#title' => 'Repair #' . ($i + 1),
	  '#rows' => 2,
	  '#attributes' => array('class' => array('repairs-textarea')),
	  '#default_value' => '',
	  '#prefix' => '<div class="repair-textarea">',
	  '#suffix' => '</div>',
	  //'#disabled' => TRUE,
	);

	$form['repair_fieldset']['repair'][$i]['time_req'][$i] = array(
	  '#type' => 'textfield',
	  '#title' => 'Time required',
	  '#description' => t('Enter the Time Required.'),
	  '#size' => 25,
	  '#attributes' => array('class' => array('time-textfield')),
	  '#default_value' => '',
	  '#prefix' => '<div class="repair-time">',
	  '#suffix' => '</div>',
	  //'#disabled' => TRUE,
	);

  }

  $form['start_row_3'] = array('#markup' => '<div class="row" id="row-3">'); // Start row_3

  // Submit button to add more repairs text fields
  $form['add_repair'] = array(
	'#type' => 'submit',
	'#value' => 'Add repair',
//	'#executes_submit_callback' => FALSE,
	'#submit' => array('repair_order_add_repair_textfield'),
	'#ajax' => array(
	  'callback' => 'ajax_repair_order_callback',
	  'wrapper' => 'ajax-repair-container',
	  'method' => 'replace',
	  'effect' => 'fade',
	),
	'#prefix' => '<div class="add">',
	'#suffix' => '</div>',
  );
  // Submit button to remove repairs text fields
  $form['remove_repair'] = array(
	'#type' => 'submit',
	'#value' => 'Remove repair',
//	'#executes_submit_callback' => FALSE,
	'#submit' => array('repair_order_remove_repair_textfield'),
	'#ajax' => array(
	  'callback' => 'ajax_repair_order_callback',
	  'wrapper' => 'ajax-repair-container',
	  'method' => 'replace',
	  'effect' => 'fade',
	),
	'#prefix' => '<div class="remove">',
	'#suffix' => '</div>',
  );

  $form['end_row_3'] = array('#markup' => '</div>'); // End row_3

  // Fieldset to hold all parts fields
  $form['parts_fieldset'] = array(
	'#type' => 'fieldset',
	'#title' => 'Parts Required',
	'#attributes' => array('class' => array('parts-fieldset')),
	'#prefix' => '<div id="ajax-parts-container">',
	'#suffix' => '</div>',
  );

  // $form_state['parts_req'] to determine the number of textfields to build.
  if (empty($form_state['parts_fieldset']['parts_req'])) {
	$form_state['parts_fieldset']['parts_req'] = 1;
  }

  $total_cost = 0; // Calculate the total cost
  for ($i = 0; $i < $form_state['parts_fieldset']['parts_req']; $i++) {

	global $total_cost;

	$form['parts_fieldset']['parts'][$i]['parts_req'][$i] = array(
	  '#type' => 'textarea',
	  '#title' => 'Part #' . ($i + 1),
	  '#rows' => 2,
	  '#attributes' => array('class' => array('parts-textfield')),
	  '#default_value' => '',
	  '#prefix' => '<div class="parts-textarea">',
	  '#suffix' => '</div>',
	  //'#disabled' => TRUE,
	);
	$form['parts_fieldset']['parts'][$i]['parts_price'][$i] = array(
	  '#type' => 'textfield',
	  '#title' => 'Price $',
	  '#description' => t('Enter the Part Cost.'),
	  '#size' => 25,
	  '#default_value' => 0,
	  '#attributes' => array('class' => array('price-textfield')),
	  '#ajax' => array(
		'callback' => 'local_app_ajax_change_price_field_callback',
		'wrapper' => 'local_app_change_price_wrapper',
		'effect' => 'fade',
	  ),
	  '#prefix' => '<div class="price">',
	  '#suffix' => '</div>',
	);

	 //Lets add up the total after ajax rebuilds the form and replaces the total price text.
	$total_cost = $total_cost + @$form_state['values']['parts_fieldset']['parts'][$i]['parts_price'][$i];
  }

  $form['start_row_4'] = array('#markup' => '<div class="row" id="row-4">'); // Start row_4

  // Submit button to add more repairs text fields
  $form['add_part'] = array(
	'#type' => 'submit',
	'#value' => 'Add part',
//	'#executes_submit_callback' => FALSE,
	'#submit' => array('repair_order_add_parts_textfield'),
	'#ajax' => array(
	  'callback' => 'ajax_parts_order_callback',
	  'wrapper' => 'ajax-parts-container',
	  'effect' => 'fade',
	),
	'#prefix' => '<div class="add">',
	'#suffix' => '</div>',
  );
  // Submit button to remove repairs text fields
  $form['remove_part'] = array(
	'#type' => 'submit',
	'#value' => 'Remove part',
//	'#executes_submit_callback' => FALSE,
	'#submit' => array('repair_order_remove_parts_textfield'),
	'#ajax' => array(
	  'callback' => 'ajax_parts_order_callback',
	  'wrapper' => 'ajax-parts-container',
	  'effect' => 'fade',
	),
	'#prefix' => '<div class="remove">',
	'#suffix' => '</div>',
  );


  $form['total_price'] = array(
	'#type' => 'textfield',
	'#field_prefix' => 'Price Total $',
    '#description' => t('Total price of parts.'),
	'#default_value' => isset($total_cost) ? number_format($total_cost, 2, '.', '') : 0,
	'#size' => 20,
	'#attributes' => array('class' => array('total-price-textfield')),
	//'#weight' => 20,
	'#disabled' => TRUE,
	'#prefix' => '<div id="local_app_change_price_wrapper">',
	'#suffix' => '</div>',

  );

  $form['end_row_4'] = array('#markup' => '</div>'); // End row_4



  $form['submit_button'] = array(
	'#type' => 'submit',
	'#value' => 'Submit Repair Order',
	'#executes_submit_callback' => TRUE,
  );

  return $form;
}

//*****************************************************************************//
/**
 * Implements hook_form_validate().
 * @param $form
 * @param $form_state
 */
function repair_order_form_validate($form, &$form_state) {

  // Get nid input from select field and search db for the title(part_num)
  // We also need to check to make sure there isn't already an order pending for this rental item.
  //$order_num = $form_state['values']['container']['order_num'];
  $nid = $form_state['values']['container']['unit_id'];
  $order_num = $form_state['values']['container']['order_num'];

  $results = db_select('repair_orders', 'r')
    ->fields('r', array('approved', 'completed'))
    ->condition('nid', $nid)
    ->execute();

  foreach ($results as $value) {

    $approved = $value->approved;
    $completed = $value->completed;
    // Check to see if pending order exists
    if ($approved == 'Approved' && $completed == FALSE) {
      form_set_error("container][unit_id", t('There is already a current repair order pending for this item.'));
    }
  }
  // Need the node to get the corresponding part num.
  $part_num = db_select('node', 'n')
    ->fields('n', array('title'))
    ->condition('nid', $nid)
    ->execute()->fetchField();

  // Save unit id for submit function
  $form_state['values']['container']['part_num'] = $part_num;

  // Calculate the follow-up-alert
  $date_submitted = $form_state['values']['container']['date_submitted'];
  $alert = $form_state['values']['follow_up_alert'];

  $date = date_create($date_submitted);
  date_add($date, date_interval_create_from_date_string($alert));
  $alert_date = date_format($date, 'M d, Y');
  // Add to $form_state
  $form_state['values']['follow_up_alert'] = $alert_date;

  // Lets prepare our multiple form values for the submit function.
  $repairs_req = $time_req = $parts_req = $parts_price = array();
  // Lets concatenate our
  for ($i = 0; $i < $form_state['repair_fieldset']['repairs_req']; $i++) {
    // Add the values to array so we can hand over to implode() to convert to comma separated string.
    $repairs_req[] = $form_state['values']['repair_fieldset']['repair'][$i]['repairs_req'][$i];
    $time_req[] = $form_state['values']['repair_fieldset']['repair'][$i]['time_req'][$i];
  }

  for ($i = 0; $i < $form_state['parts_fieldset']['parts_req']; $i++) {
    // Add the values to array so we can hand over to implode() to convert to comma separated string.
    $parts_req[] = $form_state['values']['parts_fieldset']['parts'][$i]['parts_req'][$i];
    $parts_price[] = number_format($form_state['values']['parts_fieldset']['parts'][$i]['parts_price'][$i], 0, '', ''); // Get rid of the decimals
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
  $form_state['values']['repair_archive_data'] = serialize($repair_data);

}

/**
 * Implements hook_form_submit().
 * @param $form
 * @param $form_state
 */
function repair_order_form_submit($form, &$form_state) {

  $test = unserialize($form_state['values']['repair_order_data']);

  global $user;
  // Start building query object
  $query = db_insert('repair_orders');
  $fields = array(
    'unit_id' => $form_state['values']['container']['part_num'],
    'nid' => $form_state['values']['container']['unit_id'],// This is actually giving the nid, its because of the select field.
    'date_submitted' => $form_state['values']['container']['date_submitted'],
    'repair_priority' => $form_state['values']['container']['repair_priority'],
    'po_num' => $form_state['values']['container']['po_num'],
    'last_job' => $form_state['values']['container']['last_job'],
    'repair_order_data' => $form_state['values']['repair_order_data'],
    'repair_archive_data' => $form_state['values']['repair_archive_data'],
    'repair_cost' => $form_state['values']['total_price'],
    'follow_up_alert' => $form_state['values']['follow_up_alert'],
    'submitted_by' => $form_state['values']['submitted_by'],
    'timestamp' => time(),
  );

  $query->fields($fields);
  //Submit to database
  $query->execute();
  // Add order number to session array.
  $fields['order_num'] = $form_state['values']['container']['order_num'];
  //Start a $_Session to store fields for repair_order_print_page
  $_SESSION['repair_data'] = $fields;

    // Get emails that have been set in the transfer request configuration page.
    $admin_mail = variable_get('transfer_admin_email');
    //$party_mail = variable_get('transfer_party_email');

    // Send email to administrator notifying him of request.
    $to = $admin_mail;
    $from = $user->mail;
    $params['order_num'] =  $form_state['values']['container']['order_num'];
    $params['date_submitted'] = $form_state['values']['container']['date_submitted'];

    $sent = drupal_mail('repair_order', 'new_repair_order', $to, language_default(), $params, $from, TRUE);
    // Show error if mail does not get sent for whatever reason.
    if(!$sent) {
      drupal_set_message(t('An error occurred and admin mail was not sent. Please notify administrator.'), 'error');
    }

  drupal_set_message(t('Thank You! Your Repair Order has been submitted.'));
  $form_state['redirect'] = 'repair_order/order_print';
}



//************* Repairs Ajax Section ******************************************//
function get_last_job_ajax_callback($form, &$form_state) {

  return $form['container']['last_job'];
}

// Ajax function called and used to rebuild form
function ajax_repair_order_callback($form, &$form_state) {

  return $form['repair_fieldset'];
}

/**
 * Submit handler for the "add repair" button.
 *
 * Increments the repairs_req fields and causes a rebuild.
 */
function repair_order_add_repair_textfield($form, &$form_state) {

  $form_state['repair_fieldset']['repairs_req']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "remove repair" button.
 *
 * Decrements the repairs_req fields and causes a form rebuild.
 */
function repair_order_remove_repair_textfield($form, &$form_state) {

  if ($form_state['repair_fieldset']['repairs_req'] > 1) {
	$form_state['repair_fieldset']['repairs_req']--;
  }
  $form_state['rebuild'] = TRUE;
}

//************* Parts Ajax Section ********************************************//
// Ajax function called and used to rebuild form
function ajax_parts_order_callback($form, &$form_state) {

  // Adding these ajax commands so we can change two elements with one call.
  // This will let us update the $form['total_price'] field when we add or remove a part.
  $commands = array();
  $commands[] = ajax_command_replace('#local_app_change_price_wrapper', drupal_render($form['total_price']));
  $commands[] = ajax_command_replace('#ajax-parts-container', drupal_render($form['parts_fieldset']));
  return array('#type' => 'ajax', '#commands' => $commands);
}

/**
 * Submit handler for the "add part" button.
 *
 * Increments the parts_req field and causes a rebuild.
 */
function repair_order_add_parts_textfield($form, &$form_state) {

  $form_state['parts_fieldset']['parts_req']++;
  $form_state['rebuild'] = TRUE;
}

/**
 * Submit handler for the "remove part" button.
 *
 * Decrements the parts_req field and causes a form rebuild.
 */
function repair_order_remove_parts_textfield($form, &$form_state) {

  if ($form_state['parts_fieldset']['parts_req'] > 1) {
	$form_state['parts_fieldset']['parts_req']--;
  }
  return $form_state['rebuild'] = TRUE;

}

//**************** Total Price Ajax Section ***********************************//
/**
 * Ajax callback for changing the total price textfield value.
 */
function local_app_ajax_change_price_field_callback($form, &$form_state) {

  //  $form_state['rebuild'] = TRUE;
  return $form['total_price'];
}

