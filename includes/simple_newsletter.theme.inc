<?php

/**
 * Implements hook_theme().
 *
 * TODO: Generalize this so n-templates could be used without
 * so much duplication
 */
function simple_newsletter_theme($existing, $type, $theme, $path) {
  $path = drupal_get_path('module', 'simple_newsletter') . '/templates/';
  return array(
    'simple_newsletter_header' => array(
      'variables' => array(
        'nl_node' => NULL,
        'utm_header_image' => NULL,
      ),
      'path' => $path,
      'template' => 'simple_newsletter_header',
    ),
    'simple_newsletter_footer' => array(
      'path' => $path,
      'template' => 'simple_newsletter_footer',
    ),
    'simple_newsletter_daily_a' => array(
      'variables' => array(
        'node' => NULL,
      ),
      'path' => $path,
      'template' => 'simple_newsletter_daily_a',
    ),
    'simple_newsletter_daily_card_a' => array(
      'variables' => array(
        'nl_node' => NULL,
        'node' => NULL,
        'index' => -1,
      ),
      'path' => $path,
      'template' => 'simple_newsletter_daily_card_a',
    ),
    'simple_newsletter_daily_ymal_a' => array(
      'variables' => array(
        'nl_node' => NULL,
        'node' => NULL,
        'index' => -1,
      ),
      'path' => $path,
      'template' => 'simple_newsletter_daily_ymal_a',
    ),
    'simple_newsletter_daily_b' => array(
      'variables' => array(
        'node' => NULL,
      ),
      'path' => $path,
      'template' => 'simple_newsletter_daily_b',
    ),
    'simple_newsletter_daily_card_b' => array(
      'variables' => array(
        'nl_node' => NULL,
        'node' => NULL,
        'index' => -1,
      ),
      'path' => $path,
      'template' => 'simple_newsletter_daily_card_b',
    ),
    'simple_newsletter_daily_plain_text' => array(
      'variables' => array(
        'node' => NULL,
      ),
      'path' => $path,
      'template' => 'simple_newsletter_daily_plain_text',
    ),
    'simple_newsletter_dedicated' => array(
      'variables' => array(
        'nl_node' => NULL,
        'node' => NULL,
        'index' => -1,
      ),
      'path' => $path,
      'template' => 'simple_newsletter_dedicated',
    ),
  );
}
