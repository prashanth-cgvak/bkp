<?php
// $Id: uc_vat.theme.inc,v 1.7 2010/12/09 15:07:46 longwave Exp $

/**
 * Format the cart contents table including VAT information on the checkout page.
 *
 * @param $show_subtotal
 *   TRUE or FALSE indicating if you want a subtotal row displayed in the table.
 * @param $order
 *   An order object to use for tax rule evaluation.
 * @return
 *   The HTML output for the cart review table.
 *
 * @ingroup themeable
 */
function theme_uc_vat_cart_review_table($show_subtotal = TRUE, $order = NULL) {
  $contents = uc_cart_get_contents();

  $tax_subtotal = array();
  $subtotal = 0;

  // Filter taxes to only include ones used in the cart.
  $taxes = array();
  if (!uc_vat_exclude_vat()) {
    $taxes_available = uc_vat_load_taxes($order);
    foreach ($contents as $item) {
      $node = node_load($item->nid);
      foreach ($taxes_available as $id => $tax) {
        if (in_array($node->type, $tax->taxed_product_types) && ($tax->shippable == 0 || $node->shippable == 1)) {
          $taxes[$id] = $tax;
          unset($taxes_available[$id]);
        }
      }
    }
  }

  // Set up table header.
  $header = array(
    array('data' => t('Qty'), 'class' => 'qty'),
    array('data' => t('Products'), 'class' => 'products'),
  );
  if ($taxes && variable_get('uc_vat_show_cart_columns', FALSE)) {
    if (!variable_get('uc_vat_hide_checkout_exclusive', FALSE)) {
      $header[] = array('data' => t('Price excl. !tax', array('!tax' => variable_get('uc_vat_name', 'VAT'))), 'class' => 'price excl-vat', 'style' => 'white-space: nowrap;');
    }
    foreach ($taxes as $id => $tax) {
      $tax_name = variable_get('uc_vat_hide_checkout_exclusive', FALSE) ? t('incl. !tax', array('!tax' => $tax->name)) : $tax->name;
      $header[] = array('data' => $tax_name, 'class' => 'price vat', 'style' => 'white-space: nowrap;');
    }
  }
  $header[] = array('data' => t('Price'), 'class' => 'price');

  // Set up table rows.
  $extra = array();
  $extra_total = 0;
  foreach ($contents as $item) {
    // Handle rows with no node data (discounts etc.) separately.
    if (!$item->nid) {
      $extra[] = array(
        $item->qty == 1 ? '' : array('data' => t('@qty&times;', array('@qty' => $item->qty)), 'class' => 'qty'),
        array('data' => check_plain($item->title), 'class' => 'products'),
        array(
          'data' => uc_currency_format($item->price * $item->qty),
          'class' => 'price',
          'colspan' => count($taxes) + (variable_get('uc_vat_hide_checkout_exclusive', FALSE) ? 1 : 2),
        ),
      );
      $extra_total += $item->price * $item->qty;
      continue;
    }

    $node = node_load($item->nid);
    $price_info = array(
      'price' => $item->price,
      'qty' => $item->qty,
    );
    $context = array(
      'type' => $order ? 'order_product' : 'cart_item',
      'revision' => 'altered',
      'subject' => array(
        'order' => $order,
        'cart' => $contents,
        'cart_item' => $item,
        'product' => $item,
        'node' => $node,
      ),
    );

    $total = uc_price($price_info, $context);
    $tax_total = array();
    foreach ($taxes as $id => $tax) {
      if (in_array($node->type, $tax->taxed_product_types) && ($tax->shippable == 0 || $node->shippable == 1)) {
        if (!isset($tax_total[$id])) {
          $tax_total[$id] = 0;
        }
        $tax_total[$id] += $total - $total / (1 + $tax->rate);
      }
    }
    $price = $total - array_sum($tax_total);
    foreach ($taxes as $id => $tax) {
      if (!isset($tax_subtotal[$id])) {
        $tax_subtotal[$id] = 0;
      }
      $tax_subtotal[$id] += $tax_total[$id];
    }
    $subtotal += $total;

    $description = check_plain($item->title) . uc_product_get_description($item);

    // Remove node from context to prevent the price from being altered.
    unset($context['subject']);
    $context['type'] = 'amount';
    $context['revision'] = 'themed-original';

    $row = array(
      array('data' => t('@qty&times;', array('@qty' => $item->qty)), 'class' => 'qty'),
      array('data' => $description, 'class' => 'products'),
    );
    if ($taxes && variable_get('uc_vat_show_cart_columns', FALSE)) {
      if (!variable_get('uc_vat_hide_checkout_exclusive', FALSE)) {
        $row[] = array('data' => uc_price($price, $context), 'class' => 'price excl-vat');
      }
      foreach ($taxes as $id => $tax) {
        $row[] = array('data' => $tax_total[$id] ? uc_price($tax_total[$id], $context) : '-', 'class' => 'price vat');
      }
    }
    $row[] = array('data' => uc_price($total, $context), 'class' => 'price');
    $rows[] = $row;
  }

  // Add the subtotals as the final rows.
  if ($show_subtotal) {
    $context = array(
      'type' => 'amount',
      'revision' => 'themed-original',
    );

    // Add rows for each tax (and subtotal excluding tax) if columns are not used.
    if ($taxes && !variable_get('uc_vat_show_cart_columns', FALSE)) {
      if (!variable_get('uc_vat_hide_checkout_exclusive', FALSE)) {
        $row = array(array('data' => '<span id="subtotal-title">' . t('Subtotal excluding !tax', array('!tax' => variable_get('uc_vat_name', 'VAT'))) . ':</span> ' . uc_price($subtotal - array_sum($tax_subtotal), $context), 'class' => 'subtotal excl-vat', 'colspan' => count($header)));
        $rows[] = array('data' => $row, 'class' => 'subtotal');
      }
      foreach ($taxes as $id => $tax) {
        $tax_name = variable_get('uc_vat_hide_checkout_exclusive', FALSE) ? t('incl. !tax', array('!tax' => $tax->name)) : $tax->name;
        $row = array(array('data' => '<span id="subtotal-title">' . $tax_name . ':</span> ' . uc_price($tax_subtotal[$id], $context), 'class' => 'subtotal vat', 'colspan' => count($header)));
        $rows[] = array('data' => $row, 'class' => 'subtotal');
      }
    }

    // Add final row, including tax columns if they are in use.
    $row = array();
    if ($taxes && variable_get('uc_vat_show_cart_columns', FALSE)) {
      if (!variable_get('uc_vat_hide_checkout_exclusive', FALSE)) {
        $row[] = array('data' => uc_price($subtotal - array_sum($tax_subtotal), $context), 'class' => 'subtotal excl-vat');
      }
      foreach ($taxes as $id => $tax) {
        $row[] = array('data' => uc_price($tax_subtotal[$id], $context), 'class' => 'subtotal vat');
      }
    }
    $row[] = array('data' => uc_price($subtotal, $context), 'class' => 'subtotal');

    // Set the label and width for the final row.
    $row[0]['data'] = '<span id="subtotal-title">' . t('Subtotal:') . '</span> ' . $row[0]['data'];
    $row[0]['colspan'] = count($header) - count($row) + 1;

    $rows[] = array('data' => $row, 'class' => 'subtotal');
  }

  if (!empty($extra)) {
    $rows = array_merge($rows, $extra);
    $subtotal += $extra_total;

    if ($show_subtotal) {
      $rows[] = array(
        array(
          'data' => '<span id="subtotal-title">' . t('Total:') . '</span> ' . uc_currency_format($subtotal),
          'class' => 'price',
          'colspan' => count($taxes) + (variable_get('uc_vat_hide_checkout_exclusive', FALSE) ? 3 : 4),
        ),
      );
      $extra_total += $item->price * $item->qty;
    }
  }

  return theme('table', $header, $rows, array('class' => 'cart-review'));
}

/**
 * Display the "excluding shipping costs" text including a link if available.
 *
 * @ingroup themeable
 */
function theme_uc_vat_excluding_shipping_costs($element = 'span') {
  if ($link = variable_get('uc_vat_suffix_shipping_link', '')) {
    $output = t('excluding !shipping', array('!shipping' => l(t('shipping costs'), $link)));
  }
  else {
    $output = t('excluding shipping costs');
  }

  return '<' . $element . ' class="price-shipping-suffix">' . $output . '</' . $element . '>';
}
