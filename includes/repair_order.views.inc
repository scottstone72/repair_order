<?php

/**
 * Implements hook_views_data().
 */
function repair_order_views_data() {
  $data = array();
  // Add table and fields to a group to help views sort the data.
  $data['repair_orders']['table']['group'] = t('Repair Orders');

  // Advertise this table as a possible base table
  $data['repair_orders']['table']['base'] = array(
    'field' => 'order_num',
    'title' => t('Repair Orders'),
    'help' => t("DB table for rental equipment."),
  );

  // Declare and join the repair_orders table to the node table
//  $data['repair_orders']['table']['join'] = array(
//    'node' => array(
//      'left_field' => 'nid',
//      'field' => 'nid',
//    ),
//  );

  // Describe the fields in our custom table to views.
  // This is the Unit_id field.
  $data['repair_orders']['unit_id'] = array(
    'title' => t('Unit ID'),
    // Help that appears on the UI
    'help' => t('The Unit ID of the rental equip item.'),
    // Info for displaying the unit_id
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    // Info for accepting a unit_id as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name field' => 'Unit Id', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'unit_id',
    ),
    // Info for accepting a unit_id as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    // Info for sorting a unit_id.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // This is the NID field.
  $data['repair_orders']['nid'] = array(
    'title' => t('Nid'),
    'help' => t('The node ID of the rental equip item.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'Nid',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // This is the order_num field.
  $data['repair_orders']['order_num'] = array(
    'title' => t('Order #'),
    'help' => t('The repair order number.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'Order Num',
      'numeric' => TRUE,
      'validate type' => 'order_num',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // This is the repair_priority field.
  $data['repair_orders']['repair_priority'] = array(
    'title' => t('Priority'),
    'help' => t('Priority level for each rental equip item.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name field' => 'Priority Level',
      'numeric' => TRUE,
      'validate type' => 'repair_priority',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // This is the PO_Num field.
  $data['repair_orders']['po_num'] = array(
    'title' => t('PO #'),
    'help' => t('Parts Order # for each repair order.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'PO Num',
      'numeric' => TRUE,
      'validate type' => 'po_num',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['repair_orders']['parts_req'] = array(
    'title' => t('Parts Req'),
    'help' => t('Parts required for repair order.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
      'name field' => 'Parts Req',
      'numeric' => TRUE,
      'validate type' => 'parts_req',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['repair_orders']['repairs_req'] = array(
    'title' => t('Repairs Req'),
    'help' => t('Repairs required on repair order.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
      'name field' => 'Repairs Req',
      'numeric' => TRUE,
      'validate type' => 'repairs_req',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['repair_orders']['time_req'] = array(
    'title' => t('Time Req'),
    'help' => t('Time required to finish repairs'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
      'name field' => 'Time Req',
      'numeric' => TRUE,
      'validate type' => 'time_req',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['repair_orders']['repair_price'] = array(
    'title' => t('Cost'),
    'help' => t('Price to do repairs'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
      'name field' => 'Repair Price',
      'numeric' => TRUE,
      'validate type' => 'repair_price',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['repair_orders']['last_job'] = array(
    'title' => t('Last Job'),
    'help' => t('Last job for equip item'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name field' => 'Last Job',
      'numeric' => TRUE,
      'validate type' => 'last_job',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['repair_orders']['return_date'] = array(
    'title' => t('Return Date'),
    'help' => t('Return date of completed repairs'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
      'name field' => 'Return Date',
      'numeric' => TRUE,
      'validate type' => 'return_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  $data['repair_orders']['approved'] = array(
    'title' => t('Approved'),
    'help' => t('Approved repairs orders'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name field' => 'Approved',
      'numeric' => TRUE,
      'validate type' => 'approved',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['repair_orders']['submitted_by'] = array(
    'title' => t('Submitted By'),
    'help' => t('Submitter of repair order'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name field' => 'Submitted By',
      'numeric' => TRUE,
      'validate type' => 'submitted_by',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['repair_orders']['uid'] = array(
    'title' => t('UID'),
    'help' => t('User ID on repair order'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'name field' => 'UID',
      'numeric' => TRUE,
      'validate type' => 'uid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['repair_orders']['date_submitted'] = array(
    'title' => t('Date Submitted'),
    'help' => t('Date repair order was submitted'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
      'name field' => 'Date Submitted',
      'numeric' => TRUE,
      'validate type' => 'date_submitted',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
  );

  $data['repair_orders']['administrator'] = array(
    'title' => t('Admin'),
    'help' => t('Administrator who approved repair order'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
      'name field' => 'Admin',
      'numeric' => TRUE,
      'validate type' => 'administrator',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['repair_orders']['admin_notes'] = array(
    'title' => t('Admin Notes'),
    'help' => t('Administrators notes'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
      'name field' => 'Admin Notes',
      'numeric' => TRUE,
      'validate type' => 'admin_notes',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  return $data;
}
