<?php
// $Id: uc_vat.admin.inc,v 1.10 2010/12/14 22:40:05 longwave Exp $

/**
 * Module configuration form.
 */
function uc_vat_settings_form($form_state) {
  $tax = variable_get('uc_vat_name', 'VAT');

  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Price display options'),
    '#collapsible' => TRUE,
  );
  $form['display']['uc_vat_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Tax name'),
    '#description' => t('If necessary, rename VAT to GST, MwSt, etc. as appropriate for your country.'),
    '#default_value' => $tax,
    '#size' => 10,
  );
  $form['display']['uc_vat_suffix_tax'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add "including !tax" to tax inclusive product prices.', array('!tax' => $tax)),
    '#default_value' => variable_get('uc_vat_suffix_tax', FALSE),
  );
  $form['display']['uc_vat_suffix_exclusive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add "price excluding !tax" to tax inclusive product prices.', array('!tax' => $tax)),
    '#default_value' => variable_get('uc_vat_suffix_exclusive', FALSE),
  );
  $form['display']['uc_vat_suffix_zero_rated'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add "no !tax applicable" to zero-rated product prices.', array('!tax' => $tax)),
    '#default_value' => variable_get('uc_vat_suffix_zero_rated', FALSE),
  );
  $form['display']['uc_vat_suffix_shipping'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add "excluding shipping costs" to shippable product prices and cart displays.'),
    '#default_value' => variable_get('uc_vat_suffix_shipping', FALSE),
  );
  $form['display']['uc_vat_suffix_shipping_link'] = array(
    '#type' => 'textfield',
    '#title' => t('Shipping costs page'),
    '#description' => t('If supplied, the "shipping costs" text will link to this page.'),
    '#default_value' => variable_get('uc_vat_suffix_shipping_link', ''),
  );
  $form['display']['uc_vat_exclude_superuser'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show prices excluding !tax to the superuser.', array('!tax' => $tax)),
    '#description' => t('By default, uid 1 has all permissions, which would include "show prices excluding VAT". This can be confusing so you can select here whether uid 1 should see prices inclusive or exclusive of !tax.', array('!tax' => $tax)),
    '#default_value' => variable_get('uc_vat_exclude_superuser', FALSE),
  );

  $form['edit'] = array(
    '#type' => 'fieldset',
    '#title' => t('Price editing options'),
    '#description' => t('Select the price fields below that you will enter inclusive of !tax.', array('!tax' => $tax)),
    '#collapsible' => TRUE,
  );
  $form['edit']['uc_vat_list_price_inclusive'] = array(
    '#type' => 'checkbox',
    '#title' => t('List price'),
    '#default_value' => variable_get('uc_vat_list_price_inclusive', FALSE),
  );
  $form['edit']['uc_vat_cost_inclusive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Cost'),
    '#default_value' => variable_get('uc_vat_cost_inclusive', FALSE),
  );
  $form['edit']['uc_vat_sell_price_inclusive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Sell price'),
    '#default_value' => variable_get('uc_vat_sell_price_inclusive', FALSE),
  );
  if (module_exists('uc_attribute')) {
    $form['edit']['uc_vat_default_attributes_inclusive'] = array(
      '#type' => 'checkbox',
      '#title' => t('Default attribute prices'),
      '#description' => t('Only applicable if Cost or Sell price are checked above. This should only be enabled in stores where all products will be taxed at a single rate.'),
      '#default_value' => variable_get('uc_vat_default_attributes_inclusive', FALSE),
    );
  }

  $form['checkout'] = array(
    '#type' => 'fieldset',
    '#title' => t('Cart and checkout options'),
    '#collapsible' => TRUE,
  );
  $form['checkout']['uc_vat_show_cart_vat'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show !tax amounts in the cart and at checkout.', array('!tax' => $tax)),
    '#default_value' => variable_get('uc_vat_show_cart_vat', FALSE),
  );
  $form['checkout']['uc_vat_show_cart_columns'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show !tax amounts in separate columns at checkout.', array('!tax' => $tax)),
    '#default_value' => variable_get('uc_vat_show_cart_columns', FALSE),
  );
  $form['checkout']['uc_vat_hide_subtotal'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide the "Subtotal" line item at checkout.'),
    '#description' => t('This should only be used in stores which do not use any other line items such as shipping or discounts.'),
    '#default_value' => variable_get('uc_vat_hide_subtotal', FALSE),
  );
  $form['checkout']['uc_vat_hide_checkout_exclusive'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide !tax exclusive prices and "Subtotal excluding !tax" line item.', array('!tax' => $tax)),
    '#description' => t('Enabling this will also prefix !tax amounts in the cart and the checkout with "incl."', array('!tax' => $tax)),
    '#default_value' => variable_get('uc_vat_hide_checkout_exclusive', FALSE),
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced options'),
    '#collapsible' => TRUE,
  );
  $form['advanced']['uc_vat_recalculate_prices'] = array(
    '#type' => 'checkbox',
    '#title' => t('Keep !tax inclusive prices the same after a rate change.', array('!tax' => $tax)),
    '#description' => t('By checking this box, any changes to the tax rate(s) will recalculate the base product and attribute prices accordingly, so the !tax inclusive prices will stay the same.', array('!tax' => $tax)),
    '#default_value' => variable_get('uc_vat_recalculate_prices', FALSE),
  );

  $form['advanced']['uc_vat_line_item_adjustment'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow per line-item tax adjustments (requires patch to uc_taxes.module)'),
    '#description' => t('If some products are taxed at different rates, taxes should not be applied at a flat rate against line items such as discounts. The VAT and uc_coupon modules now support this, but require a patch for uc_taxes to be installed from !link. To use this feature, apply the patch and check this box. If you do not apply the patch and check this box anyway, the VAT line item will be calculated incorrectly.', array('!link' => l('http://drupal.org/node/990744', 'http://drupal.org/node/990744'))),
    '#default_value' => variable_get('uc_vat_line_item_adjustment', FALSE),
  );
  if (module_exists('uc_quote')) {
    $form['advanced']['uc_vat_apply_shipping'] = array(
      '#type' => 'radios',
      '#title' => t('!tax on shipping', array('!tax' => $tax)),
      '#description' => t('This feature requires the above patch to be installed.'),
      '#options' => array(
        'normal' => t('Use shipping line item settings from the tax rate configuration pages.'),
        'highest' => t('Apply !tax at the highest rate used on items in the order.', array('!tax' => $tax)),
        'proportional' => t('Apply !tax proportional to rates used on items in the order.', array('!tax' => $tax)),
      ),
      '#default_value' => variable_get('uc_vat_apply_shipping', 'normal'),
      '#disabled' => !variable_get('uc_vat_line_item_adjustment', FALSE),
    );
  }

  return system_settings_form($form);
}
