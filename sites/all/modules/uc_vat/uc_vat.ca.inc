<?php
// $Id: uc_vat.ca.inc,v 1.2 2010/04/10 21:16:09 longwave Exp $

/**
 * Implementation of hook_ca_condition().
 */
function uc_vat_ca_condition() {
  $conditions['uc_vat_condition_delivery_country_eu'] = array(
    '#title' => t("Order shipping country is inside the EU"),
    '#category' => t('Order: Shipping address'),
    '#description' => t('Returns TRUE if the shipping country is inside the European Union.'),
    '#callback' => 'uc_vat_condition_delivery_country_eu',
    '#arguments' => array(
      'order' => array('#entity' => 'uc_order', '#title' => t('Order')),
    ),
  );
  $conditions['uc_vat_condition_billing_country_eu'] = array(
    '#title' => t("Order billing country is inside the EU"),
    '#category' => t('Order: Billing address'),
    '#description' => t('Returns TRUE if the billing country is inside the European Union.'),
    '#callback' => 'uc_vat_condition_billing_country_eu',
    '#arguments' => array(
      'order' => array('#entity' => 'uc_order', '#title' => t('Order')),
    ),
  );
  return $conditions;
}

/**
 * Check an order's delivery country is inside the EU.
 */
function uc_vat_condition_delivery_country_eu($order, $settings) {
  return in_array($order->delivery_country, uc_vat_eu_countries());
}

/**
 * Check an order's billing country is inside the EU.
 */
function uc_vat_condition_billing_country_eu($order, $settings) {
  return in_array($order->billing_country, uc_vat_eu_countries());
}

/**
 * Return an array of ISO 3166-1 numeric codes of the 27 EU countries.
 */
function uc_vat_eu_countries() {
  return array(
    40,  // Austria
    56,  // Belgium
    100, // Bulgaria
    196, // Cyprus
    203, // Czech Republic
    208, // Denmark
    233, // Estonia
    246, // Finland
    250, // France
    276, // Germany
    300, // Greece
    348, // Hungary
    372, // Ireland
    380, // Italy
    428, // Latvia
    440, // Lithuania
    442, // Luxembourg
    470, // Malta
    528, // Netherlands
    616, // Poland
    620, // Portugal
    642, // Romania
    703, // Slovakia
    705, // Slovenia
    724, // Spain
    752, // Sweden
    826, // United Kingdom
  );
}
