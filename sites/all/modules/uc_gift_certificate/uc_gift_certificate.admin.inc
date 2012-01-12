<?php
/**
 * Manage all of the gift certificates on record
 */
function uc_gift_certificate_manage() {
  drupal_add_css(drupal_get_path('module', 'uc_gift_certificate') ."/uc_gift_certificate.css");
  $query = 'SELECT gc.*, u.name AS purchaser_name, u2.name AS user_name, u.mail AS purchaser_mail, u2.mail AS user_mail
            FROM {uc_gift_certificates} AS gc
            LEFT JOIN {users} AS u on gc.purchaser_id = u.uid
            LEFT JOIN {users} AS u2 on gc.user_id = u2.uid
            WHERE 1=1';

  $args = array();
  if (empty($_SESSION['uc_gc_search_filter']['include_empty'])) {
    $query .= ' AND remaining_value > 0';
  }
  if (!empty($_SESSION['uc_gc_search_filter']['purchaser'])) {
    $query .= ' AND (u.name = "%s" OR u.mail = "%s")';
    $args[] = $_SESSION['uc_gc_search_filter']['purchaser'];
    $args[] = $_SESSION['uc_gc_search_filter']['purchaser'];
  }
  if (!empty($_SESSION['uc_gc_search_filter']['user'])) {
    $query .= ' AND (u2.name = "%s" OR u2.mail = "%s")';
    $args[] = $_SESSION['uc_gc_search_filter']['user'];
    $args[] = $_SESSION['uc_gc_search_filter']['user'];
  }
  if (!empty($_SESSION['uc_gc_search_filter']['order_id'])) {
    $query .= ' AND order_id = %d';
    $args[] = $_SESSION['uc_gc_search_filter']['order_id'];
  }

  $header = array(
    array('data' => t('Name'), 'field' => 'gc.name'),
    array('data' => t('Purchaser'), 'field' => 'purchaser_name', 'width' => '230'),
    array('data' => t('Recipient'), 'width' => '230', 'field' => 'user_name'),
    array('data' => t('Original Value'), 'width' => '80', 'field' => 'gc.original_value'),
    array('data' => t('Remaining Value'), 'width' => '80', 'field' => 'gc.remaining_value'),
    array('data' => t('Created On'), 'width' => '60', 'field' => 'gc.created_on', 'sort' => 'desc'),
    array('data' => t('Actions'), 'width' => 50),
  );

  $result = pager_query($query . tablesort_sql($header), 200, 0, NULL, $args);

  $rows = array();
  while ($row = db_fetch_object($result)) {
    $actions = array(
      l(t('edit'), "admin/store/gift_certificates/$row->certificate_id/edit"),
      l(t('delete'),"admin/store/gift_certificates/$row->certificate_id/delete"),
    );

    $rows[] = array(
                $row->name,
                theme('username', (object) array('uid' => $row->purchaser_id, 'name' => $row->purchaser_name, 'mail' => $row->purchaser_mail)),
                theme('username', (object) array('uid' => $row->user_id, 'name' => $row->user_name, 'mail' => $row->user_mail)),
                uc_currency_format($row->original_value),
                array('data' => uc_currency_format($row->remaining_value), 'class' => 'remaining_value'),
                array('data' => format_date($row->created_on, 'small'), 'class' => 'date'),
                array('data' => implode(' ', $actions), 'class' => 'actions'),
              );
  }

  // No gift certificates
  if (empty($rows)) {
    $rows[] = array(array('data' => t('No gift certificates found.'), 'colspan' => count($header)));
  }

  $output = drupal_get_form('uc_gift_certificate_search_form', $search_params);
  $output .= theme('table', $header, $rows, array('id' => 'uc_gc_manage'));
  $output .= theme('pager', NULL, 30, 0);
  return $output;
}

/**
 * Create a form for searching the discounts
 */
function uc_gift_certificate_search_form(&$form_state, $values) {
  $session = $_SESSION['uc_gc_search_filter'];
  $form = array();

  $form['filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filter gift certificates'),
    '#collapsible' => TRUE,
    '#collapsed' => empty($session),
  );

  $form['filters']['purchaser'] = array(
    '#type' => 'textfield',
    '#title' => t('Purchaser'),
    '#description' => t('Enter a username or email address.'),
    '#default_value' => $session['purchaser'],
  );
  $form['filters']['user'] = array(
    '#type' => 'textfield',
    '#title' => t('Recipient'),
    '#description' => t('Enter a username or email address.'),
    '#default_value' => $session['user'],
  );
  $form['filters']['order_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Order ID'),
    '#default_value' => $session['order_id'],
    '#size' => 15,
  );
  $form['filters']['include_empty'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include fully-used Certificates ($0 remaining)'),
    '#default_value' => $session['include_empty'],
  );
  $form['filters']['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Filter'),
  );
  if (!empty($session)) {
    $form['filters']['buttons']['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset')
    );
  }

  return $form;
}

function uc_gift_certificate_search_form_submit($form, &$form_state) {
  $op = $form_state['values']['op'];
  switch ($op) {
    case t('Filter'):
      $_SESSION['uc_gc_search_filter']['purchaser'] = $form_state['values']['purchaser'];
      $_SESSION['uc_gc_search_filter']['user'] = $form_state['values']['user'];
      $_SESSION['uc_gc_search_filter']['order_id'] = $form_state['values']['order_id'];
      $_SESSION['uc_gc_search_filter']['include_empty'] = $form_state['values']['include_empty'];
      break;
    case t('Reset'):
      $_SESSION['uc_gc_search_filter'] = array();
      break;
  }
  return 'admin/store/gift_certificates';
}

/**
 * Implementation of hook_form
 *
 * Show the form to add a new certificate
 */
function uc_gift_certificate_add_form($form_state, $certificate_id) {
  $is_add = $certificate_id == 'add';
  $msg_to_recipient = NULL;

  if (!$is_add) {
    $cert = uc_gift_certificate_load($certificate_id);

    $form['certificate_id'] = array(
      '#type' => 'value',
      '#value' => $cert->certificate_id,
    );
    if (!empty($cert->order_id)) {
      $order = uc_order_load($cert->order_id);
      $form['order_id'] = array(
        '#type' => 'item',
        '#value' => t('This certificate was created as part of order !order_link', array('!order_link' => l('#' . $cert->order_id, 'admin/store/orders/' . $cert->order_id))),
      );
      $msg_attrib = uc_attribute_load(variable_get('uc_gift_certificate_message_aid', ''));
      foreach ($order->products as $product) {
        if ($product->order_product_id == $cert->order_product_id && !empty($product->data['attributes'][$msg_attrib->name][0])) {
          $msg_to_recipient = $product->data['attributes'][$msg_attrib->name][0];
          break;
        }
      }
    }
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Gift Certificate Name'),
    '#default_value' => $cert->name,
    '#required' => true,
  );
  $form['cert_code'] = array(
    '#prefix' => t('<b>Certificate Code:</b><br/>'),
    '#value' => $cert->cert_code,
  );

  // If not using autocomplete
  if(!variable_get('uc_use_autocomplete_user_fields', true)) {
    $form['purchaser_id'] = array(
      '#type' => 'select',
      '#title' => t('Purchaser'),
      '#default_value' => $cert->purchaser_id,
      '#options' => uc_gift_certificate_load_users(),
      '#description' => t('Username of the purchaser of the gift certificate'),
    );

    $form['user_id'] = array(
      '#type' => 'select',
      '#title' => t('Recipient'),
      '#default_value' => $cert->user_id,
      '#options' => uc_gift_certificate_load_users(),
      '#description' => t('Username of the recipient of the gift certificate'),
    );
  }
  else {
    $form['purchaser_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Purchaser'),
      '#default_value' => uc_gift_certificate_username_from_uid($cert->purchaser_id),
      '#description' => t('Username of the purchaser of gift certificate'),
      '#autocomplete_path' => 'user/autocomplete',
    );

    $form['user_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Recipient'),
      '#default_value' => uc_gift_certificate_username_from_uid($cert->user_id),
      '#autocomplete_path' => 'user/autocomplete',
      '#description' => t('Username of the recipient of the gift certificate'),
    );
  }

  if ($is_add) {
    $form['new_user'] = array(
      '#type' => 'fieldset',
      '#title' => t('Create a new user'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['new_user']['new_user_checkbox'] = array(
      '#type' => 'checkbox',
      '#title' => t('Create a new user account for the recipient'),
      '#description' => t('Please check here to create a new user account for the recipient. You must then enter the email of the new user below.'),
    );
    $form['new_user']['new_user_email'] = array(
      '#type' => 'textfield',
      '#size' => 25,
      '#title' => t('New user\'s email address'),
      '#description' => t(''),
    );
  }

  $form['original_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Original value'),
    '#default_value' => $cert->original_value,
    '#size' => 25,
    '#description' => t('Original value of this gift certificate.'),
    '#required' => TRUE,
  );

  $form['remaining_value'] = array(
    '#type' => 'textfield',
    '#title' => t('Remaining value'),
    '#default_value' => $cert->remaining_value,
    '#size' => 25,
    '#description' => t('Amount left on the gift certificate.'),
    '#required' => TRUE,
  );

  $form['msg_to_recipient'] = array(
    '#type' => 'textarea',
    '#title' => t('Message to recipient'),
    '#default_value' => $msg_to_recipient,
  );
  $form['mail_user_checkbox'] = array(
    '#type' => 'checkbox',
    '#title' => t('Email the recipient'),
    '#description' => t('Check here to email the recipient the gift certificate.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

/**
 * Implementation of hook_form_validate()
 */
function uc_gift_certificate_add_form_validate($form, &$form_state) {
  $use_auto = variable_get('uc_use_autocomplete_user_fields', true);
  $is_new_user = $form_state['values']['new_user_checkbox'];
  if ($use_auto) {
    $form_state['values']['purchaser_id'] = uc_gift_certificate_uid_from_username($form_state['values']['purchaser_id']);
    $form_state['values']['user_id'] =  uc_gift_certificate_uid_from_username($form_state['values']['user_id']);
  }

  if (!db_result(db_query('SELECT uid FROM {users} WHERE uid != 0 AND uid = %d', $form_state['values']['purchaser_id']))) {
    form_set_error('purchaser_id', t('Purchaser is not a valid user.'));
  }
  if (!$is_new_user && !db_result(db_query('SELECT uid FROM {users} WHERE uid != 0 AND uid = %d', $form_state['values']['user_id']))) {
    form_set_error('user_id', t('Recipient is not a valid user.'));
  }
  if (!is_numeric($form_state['values']['original_value'])) {
    form_set_error('original_value', t('Original value is not a valid amount.'));
  }
  if (!is_numeric($form_state['values']['remaining_value'])) {
    form_set_error('remaining_value', t('Remaining value is not a valid amount.'));
  }
}

/**
 * Implementation of hook_form_submit()
 * Adding a new gift certificate
 */
function uc_gift_certificate_add_form_submit($form, &$form_state) {
  $use_auto = variable_get('uc_use_autocomplete_user_fields', true);
  $is_new_cert = !isset($form_state['values']['certificate_id']);
  $is_new_user = $form_state['values']['new_user_checkbox'];
  $do_mail = $form_state['values']['mail_user_checkbox'];

  // Make sure we don't generate a new cert code if we are editing an existing certificate.
  if (!isset($form_state['values']['cert_code'])) {
    $cert_code = uc_gift_certificate_mKey();
  }
  else {
    $cert_code = $form_state['values']['cert_code'];
  }

  if ($is_new_cert) {
    $form_state['values']['created_on'] = time();
    $success = drupal_write_record('uc_gift_certificates', $form_state['values']);
    $certificate_id = db_last_insert_id('uc_gift_certificates', 'certificate_id');
    $cert_code = $certificate_id."-".$cert_code;
    db_query("UPDATE {uc_gift_certificates} SET cert_code = '%s' WHERE certificate_id = %d", $cert_code, $certificate_id);
  }
  else {
    $certificate_id = $form_state['values']['certificate_id'];
    $success = drupal_write_record('uc_gift_certificates', $form_state['values'], 'certificate_id');
  }

  if ($success) {
    if ($is_new_user) {
      $mail = $form_state['values']['new_user_email'];
    }
    else {
      $cert = uc_gift_certificate_load($certificate_id);
      $user = user_load(array('uid' => $cert->user_id));
      $mail = $user->mail;
    }

    uc_gift_certificate_issue_cert($mail, $certificate_id);
    if ($do_mail) {
      uc_gift_certificate_mail_cert($mail, $certificate_id, $form_state['values']['msg_to_recipient']);
    }

    if ($is_new_cert) {
      drupal_set_message('Successfully added the gift certificate.');
    }
    else {
      drupal_set_message('Successfully updated the certificate.');
    }
  }

  drupal_goto('admin/store/gift_certificates');
}

/**
 * Confirm certificate deletion
 */
function uc_gift_certificate_delete_confirm($form_state, $cid) {
 $form['cid'] = array('#type' => 'value', '#value' => $cid);

 return confirm_form($form,
  t('Confirm Deletion of Gift Certificate %cid', array('%cid' => $cid)),
    'admin/store/gift_certificates',
  t('Deleting this gift certificate is irreversible.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Delete gift certificate
 */
function uc_gift_certificate_delete_confirm_submit($form, &$form_state) {
  $cert_id = $form_state['values']['cid'];
  $query = "DELETE FROM {uc_gift_certificates} WHERE certificate_id = '" . $cert_id . "'";
  if (db_query($query)) {
    drupal_set_message("Gift Certificate Deleted Successfully");
  }
  else {
    drupal_set_message("Error deleting gift certificate");
  }
  $form_state['redirect'] = 'admin/store/gift_certificates';
}

/**
 * Hook_settings
 */
function uc_gift_certificate_settings() {
  $form['email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Email settings'),
    '#description' => t('You can customize the content of the gift certificate email by copying uc_gift_certificate.tpl.php to your theme directory.'),
  );
  $form['email']['uc_gc_email_new_user_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Email subject for a new user'),
    '#default_value' => variable_get('uc_gc_email_new_user_subject', uc_gift_certificate_get_setting_defaults('uc_gc_email_new_user_subject')),
    '#description' => t('Email subject for a gift certificate to a new user'),
  );
  $form['email']['token_help'] = array(
    '#type' => 'fieldset',
    '#title' => t('Replacement patterns'),
    '#description' => t('You can make use of the replacement patterns in the subjects and the template file.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  foreach (array('global', 'gift_certificate', 'user') as $name) {
    $form['email']['token_help'][$name] = array(
      '#type' => 'fieldset',
      '#title' => t('@name replacement patterns', array('@name' => drupal_ucfirst($name))),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form['email']['token_help'][$name]['content'] = array(
      '#value' => theme('token_help', $name),
    );
  }
  $form['email']['uc_gc_email_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Email subject for an existing user'),
    '#default_value' => variable_get('uc_gc_email_subject', uc_gift_certificate_get_setting_defaults('uc_gc_email_subject')),
    '#description' => t('Email subject for a gift certificate to an existing user'),
  );

  $result = db_query('SELECT * FROM {uc_attributes} ORDER BY ordering, name');
  $options = array();
  while ($attrib = db_fetch_object($result)) {
    $options[$attrib->aid] = $attrib->name;
  }
  $form['uc_gift_certificate_message_aid'] = array(
    '#type' => 'select',
    '#title' => t('"Message to Recipient" attribute'),
    '#default_value' => variable_get('uc_gift_certificate_message_aid', ''),
    '#options' => $options,
    '#description' => t('Choose which attribute you are using for the "Message to Recipient" value.'),
    '#required' => TRUE,
  );
  $form['uc_gift_certificate_email_aid'] = array(
    '#type' => 'select',
    '#title' => t('"Recipient\'s Email Address" attribute'),
    '#default_value' => variable_get('uc_gift_certificate_email_aid', ''),
    '#options' => $options,
    '#description' => t('Choose which attribute you are using for the "Recipient\'s Email Address" value.'),
    '#required' => TRUE,
  );

  $options = array();
  foreach (uc_order_status_list('general') as $status) {
    $options[$status['id']] = $status['title'];
  }
  foreach (uc_order_status_list('specific') as $status) {
    $options[$status['id']] = $status['title'];
  }
  $form['uc_gift_certificate_order_status_issue'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Order Status for Issuing'),
    '#default_value' => variable_get('uc_gift_certificate_order_status_issue', uc_gift_certificate_get_setting_defaults('uc_gift_certificate_order_status_issue')),
    '#options' => $options,
    '#description' => t('Choose what the order status must be in order the for the gift certificate to be issued (upon purchase).'),
  );
  $form['uc_gift_certificate_order_status_deduct'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Order Status for Deducting'),
    '#default_value' => variable_get('uc_gift_certificate_order_status_deduct', uc_gift_certificate_get_setting_defaults('uc_gift_certificate_order_status_deduct')),
    '#options' => $options,
    '#description' => t('Choose what the order status must be in order the for the gift certificate to be deducted (upon using it).'),
  );

  $form['uc_use_autocomplete_user_fields'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use Autocomplete Fields'),
    '#default_value' => variable_get('uc_use_autocomplete_user_fields', true),
    '#description' => t('Use autocomplete fields for choosing which users are associated with a given gift certificate.'),
  );
  return system_settings_form($form);
}

/**
 * Issue a new gift certificate
 */
function uc_gift_certificate_issue() {
  global $user;

  $num_rows = db_result(db_query("SELECT COUNT(*) FROM {uc_gift_certificates} WHERE purchaser_id = %d", $user->uid));
  $result = db_query("SELECT * FROM {uc_gift_certificates} WHERE purchaser_id = %d", $user->uid);
  $rows = array();

  while ($row = db_fetch_object($result)) {
    if ($num_rows == 1) {
      drupal_goto("gift_certificates/$row->certificate_id/issue");
    }
    $rows[] = array($row->name, $row->remaining_value, l(t('send'), "gift_certificates/$row->certificate_id/issue"));
  }
  $header = array(array('data' => 'Name', 'width' => '100'),  array('data' => 'Value', 'width' => '60'),  array('data' => '', 'width' => 50));
  $output .= theme('table',$header, $rows);

  if ($output == null) {
    $output = "<p>There are currently no gift certificates in the system.</p>";
  }
  return $output;
}
