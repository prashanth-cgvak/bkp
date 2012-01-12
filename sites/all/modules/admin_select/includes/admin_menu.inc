<?php
/**
 * @file
 * Administration menu module integration.
 */

if (!function_exists('admin_menu_admin_select_info')) {
  /**
   * Implements hook_admin_select_info() on behalf of admin_menu.module.
   */
  function admin_menu_admin_select_info() {
    $info = array();

    $info['admin_menu'] = array(
      'title' => t('Administration menu module'),
      'access arguments' => array('access administration menu'),
      'suppress callback' => 'admin_menu_suppress',
    );

    return $info;
  }
}
