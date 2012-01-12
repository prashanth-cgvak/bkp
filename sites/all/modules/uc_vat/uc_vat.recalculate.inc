<?php
// $Id: uc_vat.recalculate.inc,v 1.2 2010/09/20 15:44:02 longwave Exp $

/**
 * If the VAT rate changes, recalculate prices so the VAT inclusive price stays the same.
 */
function uc_vat_recalculate_prices($form, &$form_state) {
  $old_rate = $form['rate']['#default_value'];
  $new_rate = $form_state['values']['rate'];
  $tax_name = variable_get('uc_vat_name', 'VAT');

  if ($old_rate == $new_rate) {
    // Don't do anything if the rate didn't change.
    return;
  }

  $types = "'". implode("','", array_filter($form_state['values']['taxed_product_types'])) ."'";

  // Set up old and new tax arrays.
  $old_taxes = uc_vat_load_taxes();
  $new_taxes = array();
  foreach ($old_taxes as $id => $tax) {
    $new_taxes[$id] = clone $tax;
    if ($id == $form_state['values']['id']) {
      $new_taxes[$id]->rate = $new_rate;
    }
  }

  // Find nodes to be recalculated.
  $result = db_query("SELECT n.vid, p.shippable, p.list_price, p.cost, p.sell_price FROM {node} AS n INNER JOIN {uc_products} AS p ON n.vid = p.vid WHERE n.type IN ($types)");
  $count = 0;
  while ($node = db_fetch_object($result)) {
    foreach (_uc_vat_product_fields() as $field) {
      // Add the old rates to get the VAT inclusive prices.
      foreach ($old_taxes as $tax) {
        if ($tax->shippable == 0 || $node->shippable == 1) {
          $node->{$field} *= 1 + $tax->rate;
        }
      }

      // Remove the new rates to get the new VAT exclusive prices.
      foreach (array_reverse($new_taxes) as $tax) {
        if ($tax->shippable == 0 || $node->shippable == 1) {
          $node->{$field} /= 1 + $tax->rate;
        }
      }
    }

    // Update prices in the database.
    db_query("UPDATE {uc_products} SET list_price = %f, cost = %f, sell_price = %f WHERE vid = %d", $node->list_price, $node->cost, $node->sell_price, $node->vid);
    $count++;
  }

  if ($count > 0) {
    drupal_set_message(format_plural($count, 'Recalculated the !tax exclusive price of 1 product.', 'Recalculated the !tax exclusive price of @count products.', array('!tax' => $tax_name)));
  }

  if (module_exists('uc_attribute')) {
    // Find product class attribute options to be recalculated.
    $result = db_query("SELECT pcid, oid, cost, price FROM {uc_class_attribute_options} WHERE pcid IN ($types)");
    $count = 0;
    while ($option = db_fetch_object($result)) {
      foreach (_uc_vat_attribute_fields() as $field) {
        // Add the old rates to get the VAT inclusive prices.
        foreach ($old_taxes as $tax) {
          $option->{$field} *= 1 + $tax->rate;
        }

        // Remove the new rates to get the new VAT exclusive prices.
        foreach (array_reverse($new_taxes) as $tax) {
          $option->{$field} /= 1 + $tax->rate;
        }
      }

      // Update prices in the database.
      db_query("UPDATE {uc_class_attribute_options} SET cost = %f, price = %f WHERE pcid = '%s' AND oid = %d", $option->cost, $option->price, $option->pcid, $option->oid);
      $count++;
    }

    if ($count > 0) {
      drupal_set_message(format_plural($count, 'Recalculated the !tax exclusive price of 1 product class attribute option.', 'Recalculated the !tax exclusive price of @count product class attribute options.', array('!tax' => $tax_name)));
    }

    // Find product attribute options to be recalculated.
    $result = db_query("SELECT n.nid, o.oid, o.cost, o.price FROM {node} n INNER JOIN {uc_product_options} o ON n.nid = o.nid WHERE n.type IN ($types)");
    $count = 0;
    while ($option = db_fetch_object($result)) {
      foreach (_uc_vat_attribute_fields() as $field) {
        // Add the old rates to get the VAT inclusive prices.
        foreach ($old_taxes as $tax) {
          $option->{$field} *= 1 + $tax->rate;
        }

        // Remove the new rates to get the new VAT exclusive prices.
        foreach (array_reverse($new_taxes) as $tax) {
          $option->{$field} /= 1 + $tax->rate;
        }
      }

      // Update prices in the database.
      db_query("UPDATE {uc_product_options} SET cost = %f, price = %f WHERE nid = %d AND oid = %d", $option->cost, $option->price, $option->nid, $option->oid);
      $count++;
    }

    if ($count > 0) {
      drupal_set_message(format_plural($count, 'Recalculated the !tax exclusive price of 1 attribute option.', 'Recalculated the !tax exclusive price of @count attribute options.', array('!tax' => $tax_name)));
    }
  }
}
