<?php
/**
 * Implements hook_form_FORM_ID_alter().
 */
function simple_newsletter_form_newsletter_node_form_alter(&$form, &$form_state, $form_id) {

  // Setup buttons.
  // If node is not saved, these buttons will not show.
  if(!empty($form['nid']['#value'])){
    $form['send_test_email'] = array(
      '#type' => 'submit',
      '#value' => t('Send Test Email'),
      '#submit' => array('simple_newsletter_send_to_sailthru'),
      '#weight' => 35,
    );

    $form['send_to_sailthru'] = array(
      '#type' => 'submit',
      '#value' => t('Send To Sailthru'),
      '#submit' => array('simple_newsletter_send_to_sailthru'),
      '#weight' => 36,
    );

    $form['preview_a'] = array(
      '#type' => 'submit',
      '#value' => t('Preview A'),
      '#submit' => array('simple_newsletter_preview_a'),
      '#weight' => 37,
    );

    $form['preview_b'] = array(
      '#type' => 'submit',
      '#value' => t('Preview B'),
      '#submit' => array('simple_newsletter_preview_b'),
      '#weight' => 38,
    );

    $form['preview_dedicated'] = array(
      '#type' => 'submit',
      '#value' => t('Preview Dedicated'),
      '#submit' => array('simple_newsletter_preview_dedicated'),
      '#weight' => 39,
    );
  }

  // Show/Hide fields depending on the Newsletter type
  $fields_states = array(
    'Daily' => array(
      'field_article_card',
      'preview_a',
      'preview_b',
    ),
    'Dedicated' => array(
      'field_header_image',
      'field_title_body',
      'field_newsletter_body',
      'field_button_style',
      'field_label_link',
      'preview_dedicated',
    ),
  );
  foreach ($fields_states as $type => $fields) {
    foreach ($fields as $field) {
      $form[$field]['#states'] = array(
        'visible' => array(
          ':input[name="field_email_type[und]"]' => array('value' => $type),
        ),
      );
    }
  }
  // Disable wysiwyg on body
  $form['body'][LANGUAGE_NONE][0]['#wysiwyg'] = FALSE;
  $form['body'][LANGUAGE_NONE][0]['#format'] = 'plain_text';
  $form['body'][LANGUAGE_NONE][0]['#rows'] = 5;

  // Get size of ief array
  $ief_arr_size = count($form['field_article_card'][LANGUAGE_NONE]['entities'])-5;
  for($delta = 0; $delta < $ief_arr_size; $delta++){
    // Add ajax form autocomplete to each ief

  }
  return $form;
}

function simple_newsletter_after_build(&$form, &$form_state){

  $ajax_path = url(drupal_get_path('module', 'simple_newsletter') . '/theme/js/simple_newsletter_ajax.js', array('absolute' => TRUE));

  drupal_add_js($ajax_path);
  return $form;
}


/**
 * Perform alterations before an entity form is included in the IEF widget.
 *
 * @param $entity_form
 *   Nested array of form elements that comprise the entity form.
 * @param $form_state
 *   The form state of the parent form.
 */
function simple_newsletter_inline_entity_form_entity_form_alter(&$entity_form, &$form_state) {

  if ($entity_form['#entity_type'] === 'node' && $entity_form['#bundle'] === 'card') {
    $entity_form['#after_build'][] = 'simple_newsletter_after_build';
    // Get the # of the IEF we are working on
    $delta = $entity_form['#parents'][3];
    // Make an id from that delta.
    $card_form_id = 'card_form_' . $delta;
    //dpm($entity_form,'entity form');
    if (empty($entity_form['#entity']->nid)){
      $entity_form['actions']['ief_edit_save']['#access'] = FALSE;
      $entity_form['actions']['ief_edit_cancel']['#access'] = FALSE;

      // Ajax the ief for adding items.
//      $entity_form['field_content'][LANGUAGE_NONE][0]['target_id']['#ajax'] = array(
//        'callback' => 'simple_newsletter_add_overrides',
//        'event' => 'autocompleteSelect',
//        'wrapper' => $card_form_id,
//      );

    } else {
      // Ajax the ief only for existing newsletter items.
      $entity_form['field_content'][LANGUAGE_NONE][0]['target_id']['#ajax'] = array(
        'callback' => 'simple_newsletter_overrides',
        'event' => 'autocompleteSelect',
        'wrapper' => $card_form_id,
      );
    }

    // Wrap the ief
    $entity_form['#prefix'] = '<div id="' . $card_form_id . '">';
    $entity_form['#suffix'] = '</div>';

    // Fix the Blurb
    // Disable wysiwyg on body
    $entity_form['field_blurb_override'][LANGUAGE_NONE][0]['#wysiwyg'] = FALSE;
    $entity_form['field_blurb_override'][LANGUAGE_NONE][0]['#format'] = 'plain_text';
    $entity_form['field_blurb_override'][LANGUAGE_NONE][0]['#rows'] = 5;
    //dpm($entity_form, 'entity form');

  }
}

function simple_newsletter_field_widget_form_alter(&$element, &$form_state, $context) {
  global $user;

  if (isset($element['#field_name']) && 'field_article_card' == $element['#field_name']) {


    // Do it only where no default value is set
    $entityList = array();
    if (!isset($element['entities'][0])) {
      $delta = 0;
      // Starting with new set of empty cards
      //$cloneList = array(...); // Put here nids you want to prepopulate with

      for ($i = 0; $i < 10; $i++) {
        $newEntity = entity_create('node', array('type' => 'card'));
        $newEntity->uid = $user->uid;
        $newEntity->title = 'Article Card ' . $i;
        $entityList[$i] = $newEntity;
      }
    }
    // In the "invisible" form, find out the part that really helps us here
    foreach ($form_state['inline_entity_form'] as $keyState => $formStateItem) {
      if ('card' == $formStateItem['settings']['bundles'][0]) {
        break;
      }
    }

    // Setup bundle for auto open each form
    $settings = inline_entity_form_settings($context['field'], $context['instance']);
    $bundle = reset($settings['create_bundles']);
    $controller = inline_entity_form_get_controller($context['instance']);
    $parents = array_merge($element['#field_parents'], array(
      $element['#field_name'],
      $element['#language']
    ));
    $parent_langcode = entity_language($element['#entity_type'], $element['#entity']);


    // Modify the visible part


    foreach ($entityList as $delta => $entity) {
      // The whole list can be found in inline_entity_form.module, around line 350

      $element['entities'][$delta] = array();
      $element['entities'][$delta]['#entity'] = $entity;
      $element['entities'][$delta]['#needs_save'] = TRUE;
      $element['entities'][$delta]['#weight'] = $delta;

      $element['entities'][$delta]['delta'] = array(
        '#type' => 'value',
        '#value' => $delta,
      );
      // Add the appropriate form.
      $element['entities'][$delta]['form'] = array(
        '#type' => 'container',
        '#attributes' => array('class' => array('ief-form', 'ief-form-row')),
        '#op' => 'edit',
        '#parents' => array_merge($parents, array('entities', $delta, 'form')),
        '#entity' => $entity,
        '#entity_type' => $settings['entity_type'],
        '#parent_language' => $parent_langcode,
        '#ief_id' => $keyState,
        '#ief_row_delta' => $delta,

      );
      $element['entities'][$delta]['form'] += inline_entity_form_entity_form($controller, $element['entities'][$delta]['form'], $form_state);

      $form_state['inline_entity_form'][$keyState]['entities'][$delta] = array(
        'entity' => $entity,
        'weight' => $delta,
        'form' => 'add',
        'form settings' => array(
          'bundle' => $bundle,
        ),
        'needs_save' => FALSE,

      );

      $element['entities'][$delta]['form']['field_blurb_override'][LANGUAGE_NONE][0]['#wysiwyg'] = FALSE;

      if (empty($element['#entity']->nid)){
        $element['actions']['ief_add']['#access'] = FALSE;
      }

    }
  }
}

/**
 * Implements hook_node_prepare().
 */
function simple_newsletter_node_prepare($node) {
  if (empty($node->nid) && !isset($node->path['pathauto']) && in_array($node->type, array('newsletter', 'card'))) {
    // Default the Pathauto checkbox to be unchecked for new page nodes.
    $node->path['pathauto'] = FALSE;
  }
}

/**
 * Callback function for simple_newsletter_form_newsletter_node_form_alter.
 */
function simple_newsletter_send_test_email($form, &$form_state) {

  $type = $form['field_email_type'][LANGUAGE_NONE]['#value'];
  $title = $form['title']['#value'];



}

/**
 * Callback function for simple_newsletter_form_newsletter_node_form_alter.
 */
function simple_newsletter_send_to_sailthru($form, &$form_state) {


  $type = $form['field_email_type'][LANGUAGE_NONE]['#value'];
  $title = $form['title']['#value'];
  $trigger = $form_state['triggering_element']['#value'];
  // Get the env.
  $env = empty($_SERVER['PANTHEON_ENVIRONMENT']) ? "local" : $_SERVER['PANTHEON_ENVIRONMENT'];
  $st_send_array = array();
  $st_send_array['type'] = $type;
  $list = '';
  $scheduled_time = NULL;
  if ($trigger === 'Send Test Email') {
    // We want to send a test email a & b to a test list via ST.
    $st_send_array['list'] = 'Greatist Test List';
    $st_send_array['scheduled_time'] = 'now';
  }
  else {
    // We want to schedule a blast campaign.
    $list = 'Greatist Team';
    $scheduled_time = NULL;
    $st_send_array['list'] = 'Greatist Team';
    $st_send_array['scheduled_time'] = NULL;
  }


  if ($type === 'Daily') {
    $st_send_array['name'] = 'Greatist Daily - ' . $title . ' (Version A)';
  }
  else {
    $st_send_array['name'] = 'Greatist Dedicated - ' . $title;
  }
  if ($env !== 'live' || $trigger === 'Send Test Email') {
    $st_send_array['name'] .= ' (TEST 2.0)';
  }

  $st_send_array['from_name'] = 'Greatist';
  $st_send_array['from_email'] = 'daily@greatist.com';

  $st_send_array['subject'] = $title;

  if ($type === 'Daily') {
    $template_name = 'daily_a';
  }
  else {
    $template_name = 'dedicated';
  }
  $st_send_array['template_name'] = $template_name;
  $nid = $form['nid']['#value'];
  $node = node_load($nid);
  $st_send_array['content_html'] = simple_newsletter_content_html($template_name, $node);
  $daily_plain_text = array(
    '#theme' => 'simple_newsletter_daily_plain_text',
    '#node' => $node,
  );
  $daily_plain_text = render($daily_plain_text);
  $st_send_array['content_text'] = $daily_plain_text;
  $options = array(
    'status' => 'draft',
    'is_link_tracking' => TRUE,
  );
  $st_send_array['options'] = $options;

  // Send to Sailthru.
  $response = simple_newletter_st_client_send($st_send_array);

  if($type === "Daily"){
    // Now send b.
    $st_send_array['name'] .= ' (TEST 2.0)';
    $st_send_array['name'] = 'Greatist Daily - ' . $title . ' (Version B)';
    if ($env !== 'live' || $trigger === 'Send Test Email') {
      $st_send_array['name'] .= ' (TEST 2.0)';
    }
    $st_send_array['content_html'] = simple_newsletter_content_html('daily_b', $node);
    $st_send_array['content_text'] = $daily_plain_text;
    $responseb = simple_newletter_st_client_send($st_send_array);

    if (empty($response['errormsg']) && empty($responseb['errormsg'])) {
    drupal_set_message(t('Daily drafts (Version A and B) successfully sent to Sailthru!'));
 } else {
      // TODO: find better way to handle multiple error messages.
      drupal_set_message('Drafts not created due to Sailthru. Error: ' . $response['errormsg'] . ' ' . $responseb['errormsg']);
    }
  }

}

function simple_newletter_st_client_send($st_send_array = NULL) {
  if ($st_send_array){
    $sailthruClient = sailthru_client_get_client();

    try {
      $response = $sailthruClient->scheduleBlast(
        $st_send_array['name'],
        $st_send_array['list'],
        $st_send_array['scheduled_time'],
        $st_send_array['from_name'],
        $st_send_array['from_email'],
        $st_send_array['subject'],
        $st_send_array['content_html'],
        $st_send_array['content_text'],
        $st_send_array['options']
      );

      if (empty($response['errormsg'])) {

        drupal_set_message(t($st_send_array['type'] . ' draft successfully sent to Sailthru!'));

      }
      else {
        drupal_set_message('Draft not created due to Sailthru. Error: ' . $response['errormsg']);
      }
      return $response;
    } catch (Sail_Client_Exception $e) {
      drupal_set_message(t('Draft not created due to an unexpected Sailthru Error'));
    }
  }

}

/**
 *
 * Newsletter Settings Admin form.
 *
 * @return mixed
 */
function simple_newsletter_settings() {
  return;
}

/**
 * Newsletter Previews
 */
function function_newsletter_preview($form, &$form_state, $template_name) {
  unset($form_state['storage']);
  $nid = $form['nid']['#value'];
  $redirect = url('newsletter/preview/' . $template_name . '/' . $nid, array('absolute' => TRUE));
  $form_state['redirect'] = $redirect;
}

function simple_newsletter_preview_a($form, &$form_state) {
  function_newsletter_preview($form, $form_state, 'daily_a');
}

function simple_newsletter_preview_b($form, &$form_state) {
  function_newsletter_preview($form, $form_state, 'daily_b');
}

function simple_newsletter_preview_dedicated($form, &$form_state) {
  function_newsletter_preview($form, $form_state, 'dedicated');
}


function simple_newsletter_overrides(&$entity_form, &$form_state) {


  $triggering_element = $form_state['triggering_element'];
  // Parse out nid from triggering element string
  $trigger_array1 = explode("(",$triggering_element['#value']);
  $trigger_array2 = explode(")", $trigger_array1[count($trigger_array1)-1]);
  $trigger_nid = $trigger_array2[0];

  // Load target node from nid
  $target_node = node_load($trigger_nid);
  $trigger_delta = $triggering_element['#field_parents'][3];

  // Assign fields to address.

  $title_field = 'input[name="field_article_card[' . LANGUAGE_NONE . '][entities][' . $trigger_delta . '][form][title]"]';
  $blurb_field = 'textarea[name="field_article_card[' . LANGUAGE_NONE . '][entities][' . $trigger_delta . '][form][field_blurb_override][' . LANGUAGE_NONE . '][0][value]"]';
  $image_field = 'input[name="field_article_card[' . LANGUAGE_NONE . '][entities][' . $trigger_delta . '][form][field_image_override][' . LANGUAGE_NONE . '][0][fid]"]';

  // Get values from target node.
  $promo_title = $target_node->field_promo_title[LANGUAGE_NONE][0]['value'];
  $card_title = empty($promo_title) ? $target_node->title : $promo_title;
  $card_blurb = $target_node->body[LANGUAGE_NONE][0]['summary'];
  $card_image_fid = $target_node->field_image_homepage_story[LANGUAGE_NONE][0]['fid'];

  //Build Ajax commands.
  $commands[] = ajax_command_invoke($title_field, 'val', array($card_title));
  $commands[] = ajax_command_invoke($blurb_field, 'val', array($card_blurb));
  $commands[] = ajax_command_invoke($image_field, 'val', array($card_image_fid));

  $path = libraries_get_path('FirePHPCore');
  if (file_exists($path . '/fb.php')) {
    include_once $path . '/fb.php';
    include_once $path . '/FirePHP.class.php';

    dfb($form_state, 'simple_newsletter_overrides: $form_state', FirePHP::WARN);
    dfb($entity_form, 'simple_newsletter_overrides: $entity_form', FirePHP::WARN);
    dfb($card_title, 'simple_newsletter_overrides: card title', FirePHP::WARN);
    dfb($triggering_element, 'simple_newsletter_overrides: triggerelement', FirePHP::WARN);
    dfb($commands, 'simple_newsletter_overrides: commands', FirePHP::WARN);

  }

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );


}

function simple_newsletter_add_overrides(&$entity_form, &$form_state) {

  $triggering_element = $form_state['triggering_element'];
  // Parse out nid from triggering element string
  $trigger_array1 = explode("(",$triggering_element['#value']);
  $trigger_array2 = explode(")", $trigger_array1[count($trigger_array1)-1]);
  $trigger_nid = $trigger_array2[0];

  // Load target node from nid
  $target_node = node_load($trigger_nid);
  $trigger_delta = $triggering_element['#field_parents'][3];

  // Assign fields to address.
                              //field_article_card[und][entities][0][form][title]
  $title_field = 'input[name="field_article_card[' . LANGUAGE_NONE . '][entities][' . $trigger_delta . '][form][title]"]';
                                 //field_article_card[und][entities][0][form][field_blurb_override][und][0][value]
  $blurb_field = 'textarea[name="field_article_card[' . LANGUAGE_NONE . '][entities][' . $trigger_delta . '][form][field_blurb_override][' . LANGUAGE_NONE . '][0][value]"]';
                             //field_article_card[und][entities][0][form][field_image_override][und][0][fid]
  $image_field = 'input[name="field_article_card[' . LANGUAGE_NONE . '][entities][' . $trigger_delta . '][form][field_image_override][' . LANGUAGE_NONE . '][0][fid]"]';

  // Get values from target node.
  $promo_title = $target_node->field_promo_title[LANGUAGE_NONE][0]['value'];
  $card_title = empty($promo_title) ? $target_node->title : $promo_title;
  $card_blurb = $target_node->body[LANGUAGE_NONE][0]['summary'];
  $card_image_fid = $target_node->field_image_homepage_story[LANGUAGE_NONE][0]['fid'];

  //Build Ajax commands.
  $commands[] = ajax_command_invoke($title_field, 'val', array($card_title));
  $commands[] = ajax_command_invoke($blurb_field, 'val', array($card_blurb));
  $commands[] = ajax_command_invoke($image_field, 'val', array($card_image_fid));

  $path = libraries_get_path('FirePHPCore');
  if (file_exists($path . '/fb.php')) {
    include_once $path . '/fb.php';
    include_once $path . '/FirePHP.class.php';

    dfb($form_state, 'simple_newsletter_add_overrides: $form_state', FirePHP::WARN);
    dfb($entity_form, 'simple_newsletter_add_overrides: $entity_form', FirePHP::WARN);
    dfb($card_title, 'simple_newsletter_add_overrides: card title', FirePHP::WARN);
    dfb($triggering_element, 'simple_newsletter_add_overrides: triggerelement', FirePHP::WARN);
    dfb($commands, 'simple_newsletter_add_overrides: commands', FirePHP::WARN);

  }

  return array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );


}
