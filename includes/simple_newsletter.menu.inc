<?php

/**
 * Implements hook_menu().
 *
 * @return array
 */
function simple_newsletter_menu() {
  $items = array();

  $items['admin/config/gr8ist/newsletter'] = array(
    'title' => t('Newsletters'),
    'description' => t('Configure settings for the Daily, Weekly and Dedicated Newsletters.'),
    'page callback' => 'drupal_get_form',
    'page arguments' => array('simple_newsletter_settings'),
    'access callback' => TRUE,
    'file' => 'simple_newsletter.form.inc',
    'file path' => drupal_get_path('module', 'simple_newsletter') . '/includes',
    'type' => MENU_LOCAL_TASK,
    'weight' => 3,
  );

  $items['newsletter/preview/%/%'] = array(
    'title' => 'Newsletter Preview',
    'page arguments' => array(2,3),
    'type' => MENU_CALLBACK,
    'access callback' => TRUE,
    'page callback' => 'simple_newsletter_callback_newsletter_template_preview',
  );

  $items['newsletter/nodejson/%'] = array(
    'title' => 'Node JSON',
    'page arguments' => array(2),
    'type' => MENU_CALLBACK,
    'access callback' => 'user_access',
    'access arguments' => array('administer nodes'),
    'page callback' => 'simple_newsletter_node_json',
  );


  return $items;
}
