<?php

class RepairOrder {

  // Select a single repair item from database.
  public function get_repair_item($order_num = NULL) {
    $repair_item = db_select('repair_orders', 'r')
      ->fields('r')
      ->condition('order_num', $order_num)
      ->execute()
      ->fetchAssoc();
    return $repair_item;
  }

  // Repair orders that need to be Approved.
  // Return only the repair items from database that have null values for
  // approved and completed fields. These are orders that need to get approved.
  public function get_all_approval_repair_items() {
    $result = db_select('repair_orders', 'r')
      ->fields('r')
      ->condition('approved', 'FALSE')
      ->condition('completed', 'FALSE')
      ->orderBy('timestamp','DESC')
      ->execute();

    $order_arr = array();

    while($order = $result->fetchAssoc()) {
      $order_arr[] = $order;
    }
    return $order_arr;
  }

  // Open Repair Orders.
  // Return only the open repair items from database that have 'FALSE' values for
  // completed and 'Approved' values approved for approved fields.
  public function get_all_open_repair_items() {
    $result = db_select('repair_orders', 'r')
      ->fields('r')
      ->condition('approved', 'Approved')
      ->condition('completed', 'FALSE')
      ->orderBy('timestamp','DESC')
      ->execute();

    $open_repairs = array();

    while($order = $result->fetchAssoc()) {
      $open_repairs[] = $order;
    }
    return $open_repairs;
  }

  // All Repair orders that are completed
  public function get_all_repair_order_history() {
    $query = db_select('repair_orders', 'r')
      ->fields('r')
      ->condition('completed', 'Completed')
      ->orderBy('timestamp','DESC')
      ->execute();

    $repair_history = array();

    while($order = $query->fetchAssoc()) {
      $repair_history[] = $order;
    }
    return $repair_history;
  }

  // Get all Repair Orders that are completed and are a certain Unit Id.
  public function get_repair_order_history($unit_id) {
      $query = db_select('repair_orders', 'r')
        ->fields('r')
        ->condition('completed', 'Completed')
        ->condition('unit_id', $unit_id)
        ->orderBy('timestamp','DESC')
        ->execute();

    $repair_history = array();

    while($order = $query->fetchAssoc()) {
          $repair_history[] = $order;
        }
    return $repair_history;
  }


  // Get all Unit_ID values for select field
  public function unit_id_value() {

    $unit_id_select_options = array();

    $query = db_select('node', 'n');
    $query->join('field_data_field_unit_type_', 'f', 'n.nid = f.entity_id');
    $query->groupBy('f.entity_id');
    $query->join('taxonomy_term_data', 't', 'f.field_unit_type__tid = t.tid');
    $query->groupBy('t.tid');
    $query->fields('t', array('name'));
    $query->fields('n', array('title','type', 'nid'));
    $query->fields('f', array('field_unit_type__tid'));
    $results = $query->execute();


    foreach($results as $val) {
      $unit_id_select_options[$val->name][$val->nid] = $val->title;
    }

    return $unit_id_select_options;
  }


  // Get the repair order number
  public function repair_order_num() {
    $query = db_select('repair_orders', 'ro');
    $query->addExpression('MAX(order_num)');
    $max= $query->execute()->fetchField();

    if(empty($max)) {
      $form_state['values']['container']['order_num'] = 1;
      return $form_state['values']['container']['order_num'];
    }
    else {
      $form_state['values']['container']['order_num'] = $max;
      return $form_state['values']['container']['order_num'] + 1;
    }
  }


  // Get the last job for the item selected.
  public function get_last_job($nid) {
    $query = db_select('field_data_field_location', 'l');
    $results = $query
      ->fields('l', array('field_location_value'))
      ->condition('entity_id', $nid)
      ->execute()->fetchField();

    return $results;
  }

}

