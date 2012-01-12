<?php
// $Id: uc_vat_handler_field_price.inc,v 1.2 2010/10/23 23:31:07 longwave Exp $

/**
 * Return a formatted price value to display in the View.
 */
class uc_vat_handler_field_price extends uc_product_handler_field_price {
  function option_definition() {
    $options = parent::option_definition();
    $options['show_label'] = array('default' => FALSE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['show_label'] = array(
      '#title' => t('Show price labels'),
      '#description' => t('This will show any prefixes and suffixes supplied by price handlers.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['show_label']),
      '#weight' => 3,
      '#process' => array('views_process_dependency'),
      '#dependency' => array('radio:options[format]' => array('uc_price')),
      '#dependency_count' => 1,
    );
  }

  function render($values) {
    if ($this->options['format'] == 'numeric') {
      return parent::render($values);
    }

    if ($this->options['format'] == 'uc_price') {
      $product = node_load($values->{$this->aliases['nid']});
      $context = array(
        'revision' => $this->options['revision'],
        'type' => 'product',
        'class' => array(
          'product',
          $this->field,
        ),
        'field' => $this->real_field,
        'subject' => array(
          'node' => $product,
        ),
      );
      $options = array('label' => !empty($this->options['show_label']));

      return uc_price($product->{$this->real_field}, $context, $options);
    }
  }
}
