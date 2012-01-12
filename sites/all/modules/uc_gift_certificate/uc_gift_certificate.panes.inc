<?php
/**
 * Our checkout panes
 */
function uc_gift_certificate_checkout_pane() {
  $panes[] = array(
    'id' => 'certificate_discount',
    'callback' => 'uc_gift_certificate_pane_coupon_discount',
    'title' => t('Certificate Discount'),
    'desc' => t('Allows shoppers to use a gift certificate during checkout for order discounts.'),
    'weight' => 5,
    'process' => TRUE,
  );
  $panes[] = array(
    'id' => 'gift_certificate',
    'callback' => 'uc_checkout_pane_gift_certificate',
    'title' => t('Gift Certificates'),
    'desc' => t('Allows shoppers to see that they have a gift certificate during checkout.'),
    'weight' => 5,
    'process' => TRUE,
  );

  return $panes;
}

/**
 * Checkout pane that displays how much in gift certificates a user has
 */
function uc_checkout_pane_gift_certificate($op, &$arg1, $arg2) {
  global $user;
  switch ($op) {
    case 'settings':
      $form['uc_gc_pane_msg'] = array(
        '#type' => 'textarea',
        '#title' => t('Checkout Pane Message'),
        '#default_value' => variable_get('uc_gc_pane_msg', uc_gift_certificate_get_setting_defaults('uc_gc_pane_msg')),
        '#description' => t('Gift certificate message in the checkout pane.'),
      );
      return $form;
    case 'view':
      $order = $arg1;
      // See if we ar editing an order or creating it
      $uid = empty($order->uid) ? $user->uid : $order->uid;
      if (!uc_gift_certificate_total($uid)){
        //do not show pane in this case
        return;
      }

      $cert_arr = uc_gift_certificate_load_certificates($uid);
      $total = uc_gift_certificate_total($uid);
      $description = t('');
      $value = variable_get('uc_gc_pane_msg', uc_gift_certificate_get_setting_defaults('uc_gc_pane_msg'));
      $variables = array('!tot' => uc_currency_format($total));
      $value = strtr($value, $variables);
      $contents['gift_certificate_total'] = array('#value' => $value);
      return array('description' => $description, 'contents' => $contents);
  }
}

/*
 * Checkout pane that allows the entry of a gift cert code
 */
function uc_gift_certificate_pane_coupon_discount($op, &$arg1, $arg2) {
  switch ($op) {
    case 'view':
      drupal_add_js(drupal_get_path('module', 'uc_gift_certificate') .'/uc_gift_certificate.js');
      $description = t('Enter a Gift Certificate for this order.');
      $contents['certificate_code'] = array(
        '#type' => 'textfield',
        '#title' => t('Certificate Code'),
        '#default_value' => $arg1->data['certificate_code'],
        '#size' => 25,
      );
      return array('description' => $description, 'contents' => $contents);
    case 'process':
      $arg1->data['certificate_code'] = $arg2['certificate_code'];
      return TRUE;
    case 'review':
      break;
  }
}

/**
 * NOTE: Not used - ditch it?
 * Checkout Pane callback function.
 *
 * Used to display a form in the checkout process so that customers
 * can enter discount coupons.
 */
function uc_gift_certificate_cert_checkout($op, $arg1, $arg2) {
  global $user;


//drupal_set_message('Op: '.$op.' Arg2: <pre>'.print_r($arg2, true).'</pre>');
  switch ($op) {
    case 'view':
      uc_add_js(drupal_get_path('module', 'uc_gift_certificate') .'/uc_gift_certificate.js');

      if ($arg1->data['gift_certificate'] != '') {
        $certificate = uc_gift_certificate_validate($arg1->data['gift_certificate']);
      }

      // This will
      if ($arg1->data['gift_certificate_user_value'] != '') {
        $certificate = uc_gift_certificate_validate_user($arg1->data['gift_certificate_user_value']);
      }

      if ($certificate->valid) {
        uc_add_js('$(document).ready(function() {
          if (window.set_line_item) {
            set_line_item("gift_certificate", "'. $certificate->title .'", '. -$certificate->amount .', 9);
            render_line_items();
          }
        });', 'inline');
      }

      // Start the description for the Certificates form
      $description = variable_get('uc_gift_certificate_pane_description', 'If you have a Gift Certificate code, you can enter it here. ');

      // If we are a logged-in user, calculate their total certificates.
      if ($user->uid > 0) {
        $certsTotal = uc_gift_certificate_total($user->uid);

        if ($certsTotal > 0) {
          $description .= t('<br/><br/><strong>You currently have !total</strong> worth of gift certificates in your account', array('!total' => uc_currency_format($certsTotal)));
        }

        $items = uc_cart_get_contents();
        $total = 0;

        foreach ($items AS $item) {
          $total += ($item->price) * $item->qty;
        }

        // Decide if it's a discount
        if ($total > $certsTotal) {
          $description .= t(', which you can apply as a discount using the button below.');
          $contents['certificate_discount'] = array(
            '#type' => 'button',
            '#value' => t('Apply as Discount'),
            '#attributes' => array('onclick' => " getCertDiscount(); return false;"),
          );

          $description .= '<br/><br/>';
        }
        else {
          // Certificates are more than the total, so they can cover the whole cost.
          if ($total < $certsTotal) {
            $description .= t(', which is enough to cover this entire order!</strong><br/><br/>You MUST click this in order to pay for your entire order!');
            $contents['certificate_discount'] = array(
            '#type' => 'button',
            '#value' => t('Apply to Order'),
            '#attributes' => array('onclick' => " getCertDiscount(); return false;"),
          );
          }
        }
      }

      $contents['certificate-user-value'] = array(
        '#type' => 'hidden',
        '#default_value' => 0,
      );

      // Hidden field used to get the user's total certificates
      $contents['certificate-user'] = array(
        '#type' => 'hidden',
        '#value' => $user->uid,
      );


      $order = uc_order_load($_SESSION['cart_order']);

      $contents['certificate-order-id'] = array(
        '#type' => 'hidden',
        '#default_value' => $order->order_id,
      );

      // Show the text field for entering a new Certificate code (possibly a Printed certificate)
      $contents['code'] = array(
        '#type' => 'textfield',
        '#title' => t('Gift Certificate'),
        '#default_value' => $arg1->data['gift_certificate'],
        '#size' => 25,
      );
      $contents['apply'] = array(
        '#type' => 'button',
        '#value' => t('Apply New Gift Certificate Code'),
        '#attributes' => array('onclick' => "getCert(); return false;"),
      );
      return array('description' => $description, 'contents' => $contents);

    case 'process':
      // Check to see if they are paying for the entire order with Gift Certificates
      if ($arg1->payment_method == 'zero_total') {
        $certificates = uc_gift_certificate_total($arg1->uid);

        // Get current cart total including all line_items.
        $order_total = $arg1->order_total;

        $line_items = db_result(db_query("SELECT SUM(amount) FROM {uc_order_line_items} WHERE order_id = %d AND type <> 'gift_certificate'", $arg1->order_id));
        $order_total += $line_items;

        // If the user has more certificates than the order total, update the line item to cover it (or add a new line item)
        if ($certificates >= $total) {
          $result = db_query("SELECT line_item_id FROM {uc_order_line_items} WHERE order_id = %d AND type = 'gift_certificate'", $arg1->order_id);
          if ($lid = db_result($result)) {
            uc_order_update_line_item($lid, t('Gift Certificates'), -abs($order_total));
          }
          else {
            uc_order_line_item_add($arg1->order_id, 'gift_certificate', t('Gift Certificates'), -$order_total);
          }
        }
      }

      // Otherwise, if they have entered a new code into the textfield during Checkout.. (printable Certificate)
      elseif ($arg2['code']) {
        $arg1->data['gift_certificate'] = check_plain($arg2['code']);
        $arg1->data['gift_certificate_value'] = $arg2['certificate-user-value'];

        $certificate = uc_gift_certificate_validate($arg2['code']);
        if (!$certificate->valid) {
          drupal_set_message(t('Sorry, the Certificate you tried to use is invalid or cannot be applied to this order.'), 'error');
          return FALSE;
        }

        $result = db_query("SELECT line_item_id FROM {uc_order_line_items} WHERE order_id = %d AND type = 'gift_certificate'", $arg1->order_id);
        if ($lid = db_result($result)) {
          uc_order_update_line_item($lid, $certificate->title, -abs($certificate->amount));
        }
        else {
          uc_order_line_item_add($arg1->order_id, 'gift_certificate', $certificate->title, -$certificate->amount);
        }
      }
      // Otherwise, they have hit the "Apply as Discount" button
      elseif ($arg2['certificate-user-value'] != 0) {
        drupal_set_message('Discount applied: '.$arg2['certificate-user-value']);
        $certificate = uc_gift_certificate_validate_user($arg2['certificate-user']);
        if ($certificate->valid) {
          uc_add_js('$(document).ready(function() {
            if (window.set_line_item) {
              set_line_item("gift_certificate", "'. $certificate->title .'", '. -$certificate->amount .', 9);
              render_line_items();
            }
          });', 'inline');
          $result = db_query("SELECT line_item_id FROM {uc_order_line_items} WHERE order_id = %d AND type = 'gift_certificate'", $arg1->order_id);
          if ($lid = db_result($result)) {
            uc_order_update_line_item($lid, $certificate->title, -abs($certificate->amount));
          }
          else {
            uc_order_line_item_add($arg1->order_id, 'gift_certificate', $certificate->title, -$certificate->amount);
          }
        }
      }
      return TRUE;

    case 'review':
      // If any certificates are being used in this order, show them during Cart Review
      $certificates = uc_gift_certificate_total($arg1->uid);

      foreach ($arg1->line_items AS $item) {
        if ($item['type'] == 'gift_certificate') {
          $message = '<center>'. $item['title'] .'</center>';
          return $message;
        }
      }
      break;

    case 'settings':
      return;
  }
}

