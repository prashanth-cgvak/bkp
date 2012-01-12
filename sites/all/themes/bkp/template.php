<?php

/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

//views_exposed_form__residential_search

function bkp_theme() {
    return array(
        'simplenews_newsletter_subject' => array(
            'arguments' => array('name' => "", 'title' => "", 'language' => '')
        )
    );
}

function bkp_preprocess_page(&$variables) {
    $variables['tabs'] = menu_primary_local_tasks();
    $variables['tabs2'] = menu_secondary_local_tasks();

    if(isset($variables['node'])&&$variables['node']->type=='product'){
    $variables['title'] = $variables['breadcrumb'];
    }
    return $variables;
}

function bkp_simplenews_newsletter_subject($name, $title, $language) {
    return $title;
}

