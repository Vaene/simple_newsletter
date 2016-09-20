<?php

function simple_newsletter_preprocess_page(&$vars) {
  $node = NULL;
  if(!empty($vars['node'])){
    $node = $vars['node'];
  }
  if ((!empty($node)) && ($node->type === 'newsletter')) {
    $css_path = drupal_get_path('module', 'simple_newsletter') . '/theme/css/simple_newsletter.css';
    drupal_add_css($css_path, array( "type" => "file" ));
  }
}

function simple_newsletter_social_link($name, $url, $img_url) {
  $image = theme('image', array(
    'path' => $img_url,
    'width' => 14,
    'height' => 14,
    'alt' => 'Greatist on ' . $name
  ));
  return l($image, $url, array(
    'html' => TRUE,
    'attributes' => array(
      'class' => strtolower($name)
    )
  ));
}

function simple_newlsetter_social_links() {
  $social_links = '';
  $social_links .= simple_newsletter_social_link('Facebook', 'https://www.facebook.com/greatist', GR8IST_NEWSLETTER_SOCIAL_FACEBOOK_IMG_URL);
  $social_links .= simple_newsletter_social_link('Pinterest', 'http://www.pinterest.com/greatist/', GR8IST_NEWSLETTER_SOCIAL_PINTEREST_IMG_URL);
  $social_links .= simple_newsletter_social_link('Instagram', 'http://www.instagram.com/greatist/', GR8IST_NEWSLETTER_SOCIAL_INSTAGRAM_IMG_URL);
  $social_links .= simple_newsletter_social_link('Snapchat', 'https://www.snapchat.com/add/greatistsnap', GR8IST_NEWSLETTER_SOCIAL_SNAPCHAT_IMG_URL);
  return $social_links;
}

function simple_newsletter_preprocess_simple_newsletter_header(&$vars) {
  global $base_url;
  $utm_launch_code = 'testB_20160817';
  $nl_node = $vars['nl_node'];
  $send_date = date('Y-m-d',$nl_node->field_send_date[LANGUAGE_NONE][0]['value']);
  $utm_header_image = $vars['utm_header_image'];
  if (empty($utm_header_image)) {
    $utm_header_image = simple_newsletter_utm_generate(0, $type = 'header_image', $send_date, $utm_launch_code);
  }

  // <img src="http://media.sailthru.com/35b/1k0/7/1/5776ad6f266a0.png" width="454" height="168" alt="greatist_gfy_logo.png" />
  $image = theme('image', array(
    'path' => GR8IST_NEWSLETTER_LOGO_URL,
    'width' => 227
  ));

  $link = $base_url;
  $header_link = l($image, $link, array(
    'html' => TRUE,
    'query' => $utm_header_image,
    'attributes' => array('class' => 'header-link')
  ));
  $vars['header_link'] = $header_link;
  $vars['social_links'] = simple_newlsetter_social_links();
}

function simple_newsletter_preprocess_simple_newsletter_footer(&$vars) {
  $vars['social_links'] = simple_newlsetter_social_links();
}

function simple_newsletter_preprocess_simple_newsletter_daily_card_b(&$vars) {
  $markup = '';
  $nl_node = $vars['nl_node'];
  $node = $vars['node'];
  $index = $vars['index'];

  $utm_launch_code = 'testB_20160817';

  $send_date = date('Y-m-d',$nl_node->field_send_date[LANGUAGE_NONE][0]['value']);
  $utm_image = simple_newsletter_utm_generate($index, $type = 'image', $send_date, $utm_launch_code);
  $utm_title = simple_newsletter_utm_generate($index, $type = 'title', $send_date, $utm_launch_code);
  $utm_cta = simple_newsletter_utm_generate($index, $type = 'cta', $send_date, $utm_launch_code);
  // Temp workaround until the override field Ajax works
  $node_original = node_load($node->field_content[LANGUAGE_NONE][0]['target_id']);

  $title = '';
  if (!empty($node->title)) {
    $title = $node->title;
  } else if(!empty($node->field_promo_title[LANGUAGE_NONE][0]['value'])) {
    $title = $node->field_promo_title[LANGUAGE_NONE][0]['value'];
  } else {
    $title = $node_original->title;
  };

  $image_uri = '';
  if(!empty($node->field_image_override[LANGUAGE_NONE][0]['fid'])){
    $file = file_load($node->field_image_override[LANGUAGE_NONE][0]['fid']);
    $image_uri = $file->uri;
  } else {
    $image_uri = $node_original->field_image_homepage_story[LANGUAGE_NONE][0]['uri'];
  }
  $image_style = 'newsletter-card-768x398';
  $url = 'node/' . $node_original->nid;
  if ($node_original->type === 'native_ad') {
    $image_style = 'article-card-752';
    $native_ad_url = $node_original->field_target_url[LANGUAGE_NONE][0]['url'];
    if (!empty($native_ad_url)) {
      $url = $native_ad_url;
    }
    $utm_image = simple_newsletter_utm_generate('promo_' . $node_original->title, 'native_ad', $send_date, $utm_launch_code);
  }

  $image_path = image_style_url($image_style, $image_uri);
  $image_markup = theme('image', array(
    'path' => $image_path
  ));
  $image_link = l($image_markup, $url, array(
    'html' => TRUE,
    'absolute' => TRUE,
    'query' => $utm_image,
  ));
  $markup .= $image_link;

  if ($node_original->type !== 'native_ad') {
    $partner_id = $node_original->field_partner[LANGUAGE_NONE][0]['target_id'];
    if (!empty($partner_id)) {
      $partner_node = node_load($partner_id);
      $markup .= '<div class="partner"> In Partnership With ' . $partner_node->title . '</div>';
    }

    $title_markup = '<h2>' . check_plain($title) . '</h2>';
    $title_link = l($title_markup, 'node/' . $node_original->nid, array(
      'html' => TRUE,
      'absolute' => TRUE,
      'query' => $utm_title,
    ));
    $markup .= $title_link;

    $blurb = '';
    if (!empty($node->field_blurb_override[LANGUAGE_NONE][0]['value'])) {
      $blurb = $node->field_blurb_override[LANGUAGE_NONE][0]['value'];
    }
    else {
      $blurb = $node_original->body[LANGUAGE_NONE][0]['summary'];
    }
    $description_markup = '<div class="description">' . check_plain($blurb) . '</div>';
    $markup .= $description_markup;

    if ($vars['show_cta']) {
      $cta_link = l('Read More ›', 'node/' . $node_original->nid, array(
        'attributes' => array('class' => 'cta'),
        'absolute' => TRUE,
        'query' => $utm_cta,
      ));
      $markup .= $cta_link;
    }
  }

  $vars['markup'] = $markup;
}

function simple_newsletter_preprocess_simple_newsletter_daily_card_a(&$vars) {
  $nl_node = $vars['nl_node'];
  $node = $vars['node'];
  // Temp workaround until the override field Ajax works
  $node_original = node_load($node->field_content[LANGUAGE_NONE][0]['target_id']);

  $send_date = date('Y-m-d',$nl_node->field_send_date[LANGUAGE_NONE][0]['value']);

  $utm_launch_code = 'testA_20160817';
  $index = $vars['index'];
  if ($index < 3) {
    $utm_image = simple_newsletter_utm_generate($index,'image',$send_date, $utm_launch_code);
    $utm_title = simple_newsletter_utm_generate($index,'title',$send_date,$utm_launch_code);
    $utm_cta = simple_newsletter_utm_generate($index,'cta',$send_date,$utm_launch_code);

    $image_style = 'newsletter-card-768x398';
    $url = 'node/' . $node_original->nid;
    if ($node_original->type === 'native_ad') {
      $image_style = 'article-card-752';
      $native_ad_url = $node_original->field_target_url[LANGUAGE_NONE][0]['url'];
      if (!empty($native_ad_url)) {
        $url = $native_ad_url;
      }
      $utm_image = simple_newsletter_utm_generate('promo_' . $node_original->title, 'native_ad', $send_date, $utm_launch_code);
    }

    $large_a_card_image_link = url($url, array(
      'absolute' => TRUE,
      'query' => $utm_image
    ));
    $vars['large_a_card_image_link'] = $large_a_card_image_link;

    // Partner
    $partner_id = $node_original->field_partner[LANGUAGE_NONE][0]['target_id'];
    if (!empty($partner_id)) {
      $partner_node = node_load($partner_id);
      $vars['partner'] = 'With ' . $partner_node->title;
    }

    // Get image.
    $large_a_card_img_uri = '';
    if(!empty($node->field_image_override[LANGUAGE_NONE][0]['fid'])){
      $file = file_load($node->field_image_override[LANGUAGE_NONE][0]['fid']);
      $large_a_card_img_uri = $file->uri;
    } else {
      $large_a_card_img_uri = $node_original->field_image_homepage_story[LANGUAGE_NONE][0]['uri'];
    }
    $large_a_card_img_src = image_style_url($image_style, $large_a_card_img_uri);
    $vars['large_a_card_img_src'] = $large_a_card_img_src;

    if ($node_original->type !== 'native_ad') {
      $large_a_card_title_link = url($url, array(
        'absolute' => TRUE,
        'query' => $utm_title
      ));
      $vars['large_a_card_title_link'] = $large_a_card_title_link;

      $large_a_card_cta_link = url($url, array(
        'absolute' => TRUE,
        'query' => $utm_cta
      ));
      $vars['large_a_card_cta_link'] = $large_a_card_cta_link;

      // Get Title.
      $large_a_title = '';

      if (!empty($node->title)) {
        $large_a_title = $node->title;
      } else if(!empty($node->field_promo_title[LANGUAGE_NONE][0]['value'])) {
        $large_a_title = $node_original->field_promo_title[LANGUAGE_NONE][0]['value'];
      } else  {
        $large_a_title = $node_original->title;
      };
      $vars['large_a_title'] = $large_a_title;

      // Get Summary.
      $blurb_override = $node->field_blurb_override[LANGUAGE_NONE][0]['value'];
      if (!empty($blurb_override)) {
        $large_a_summary = $blurb_override;
      } else {
        $large_a_summary = $node_original->body[LANGUAGE_NONE][0]['summary'];
      }
      $vars['large_a_summary'] = $large_a_summary;

      // CTA
      $large_a_card_cta_label = 'Read More';
      $vars['large_a_card_cta_label'] = $large_a_card_cta_label;
    }
  }
}

function simple_newsletter_preprocess_simple_newsletter_daily_ymal_a(&$vars) {
  $nl_node = $vars['nl_node'];
  $node = $vars['node'];
  $utm_launch_code = 'testA_20160817';
  $send_date = date('Y-m-d',$nl_node->field_send_date[LANGUAGE_NONE][0]['value']);

  // Temp workaround until the override field Ajax works
  $node_original = node_load($node->field_content[LANGUAGE_NONE][0]['target_id']);

  $index = $vars['index'];
  if ($index > 2) {
    // Use original node values if card values empty. Backup for Ajax.
    $ymal_a_nid = NULL;
    $ymal_a_title = '';
    $ymal_a_summary = '';

    // Get nid, this always is the original article.
    $ymal_a_nid = $node_original->nid;

    // Partner
    $partner_id = $node_original->field_partner[LANGUAGE_NONE][0]['target_id'];
    if (!empty($partner_id)) {
      $partner_node = node_load($partner_id);
      $vars['partner'] = 'With ' . $partner_node->title;
    }

    if ($node_original->type !== 'native_ad') {
      // Get Title.
      if (!empty($node->title)) {
        $ymal_a_title = $node->title;
      } else if(!empty($node->field_promo_title[LANGUAGE_NONE][0]['value'])) {
        $ymal_a_title = $node->field_promo_title[LANGUAGE_NONE][0]['value'];
      } else {
        $ymal_a_title = $node_original->title;
      };
      $vars['ymal_a_title'] = $ymal_a_title;

      // Get Summary.
      if (!empty($node->field_blurb_override[LANGUAGE_NONE][0]['value'])) {
        $ymal_a_summary = $node->field_blurb_override[LANGUAGE_NONE][0]['value'];
      } else {
        $ymal_a_summary = $node_original->body[LANGUAGE_NONE][0]['summary'];
      }
      $vars['ymal_a_summary'] = $ymal_a_summary;

      $utm_title = simple_newsletter_utm_generate($index,'title',$send_date,$utm_launch_code);
      $utm_cta = simple_newsletter_utm_generate($index,'cta',$send_date,$utm_launch_code);

      // Get title link.
      $ymal_a_title_link = url('node/' . $ymal_a_nid, array(
        'absolute' => TRUE,
        'query' => $utm_title
      ));
      $vars['ymal_a_title_link'] = $ymal_a_title_link;

      $ymal_a_cta_link = url('node/' . $ymal_a_nid, array(
        'absolute' => TRUE,
        'query' => $utm_cta
      ));
      $vars['ymal_a_cta_link'] = $ymal_a_cta_link;

      // TODO: Get this value from form.
      $ymal_a_cta_label = 'Read More';
      $vars['ymal_a_cta_label'] = $ymal_a_cta_label;
    }
    else {
      // Native Ad
      $utm_title = simple_newsletter_utm_generate('promo_' . $node_original->title, 'native_ad', $send_date, $utm_launch_code);
      $native_ad_url = $node_original->field_target_url[LANGUAGE_NONE][0]['url'];
      if (!empty($native_ad_url)) {
        $ymal_a_title_link = url($native_ad_url, array(
          'absolute' => TRUE,
          'query' => $utm_title
        ));
        $vars['ymal_a_title_link'] = $ymal_a_title_link;
      }

      $large_a_card_img_uri = '';
      if(!empty($node->field_image_override[LANGUAGE_NONE][0]['fid'])){
        $file = file_load($node->field_image_override[LANGUAGE_NONE][0]['fid']);
        $large_a_card_img_uri = $file->uri;
      } else {
        $large_a_card_img_uri = $node_original->field_image_homepage_story[LANGUAGE_NONE][0]['uri'];
      }
      $large_a_card_img_src = image_style_url('article-card-752', $large_a_card_img_uri);
      $vars['large_a_card_img_src'] = $large_a_card_img_src;
    }
  }
}

function simple_newsletter_preprocess_simple_newsletter_daily_b(&$vars) {
  $nl_node = $vars['node'];
  $utm_launch_code = 'testB_20160817';
  $nodes = array();
  foreach ($nl_node->field_article_card[LANGUAGE_NONE] as $card) {
    $nodes[] = node_load($card['target_id']);
  }
  $send_date = date('Y-m-d',$nl_node->field_send_date[LANGUAGE_NONE][0]['value']);
  $utm_header_image = simple_newsletter_utm_generate(0, $type = 'header_image', $send_date, $utm_launch_code);
  $newsletter_b_blurb = strip_tags($nl_node->body[LANGUAGE_NONE][0]['value']);
  $cards = '';
  $show_cta = TRUE;
  if (!empty($nl_node->field_hide_cta) && ($nl_node->field_hide_cta[LANGUAGE_NONE][0]['value'])) {
    $show_cta = FALSE;
  }
  foreach ($nodes as $index => $node) {
    $cards .= theme('simple_newsletter_daily_card_b', array(
      'nl_node' => $nl_node,
      'node' => $node,
      'index' => $index,
      'show_cta' => $show_cta,
    ));
  }

  $header = theme('simple_newsletter_header', array(
    'nl_node' => $nl_node,
  ));

  $footer = theme('simple_newsletter_footer');

  $vars['newsletter_b_blurb'] = $newsletter_b_blurb;
  $vars['header'] = $header;
  $vars['cards'] = $cards;
  $vars['footer'] = $footer;
}

function simple_newsletter_preprocess_simple_newsletter_daily_a(&$vars) {
  $nl_node = $vars['node'];
  $utm_launch_code = 'testA_20160817';
  $nodes = array();
  foreach ($nl_node->field_article_card[LANGUAGE_NONE] as $card) {
    $nodes[] = node_load($card['target_id']);
  }
  $send_date = date('Y-m-d',$nl_node->field_send_date[LANGUAGE_NONE][0]['value']);
  $newsletter_a_title = $nl_node->title;
  $newsletter_a_blurb = strip_tags($nl_node->body[LANGUAGE_NONE][0]['value']);
  $large_a_cards = '';
  $ymal_a_cards = '';
  foreach ($nodes as $index => $node) {
    if ($index < 3) {
      $large_a_cards .= theme('simple_newsletter_daily_card_a', array(
        'nl_node' => $nl_node,
        'node' => $node,
        'index' => $index
      ));
    }
    else {
      $ymal_a_cards .= theme('simple_newsletter_daily_ymal_a', array(
        'nl_node' => $nl_node,
        'node' => $node,
        'index' => $index
      ));
    }
  }
  $utm_header_image = simple_newsletter_utm_generate(0, $type = 'header_image', $send_date, $utm_launch_code);
  $header_image_link = url('http://greatist.com', array(
    'absolute' => TRUE,
    'query' => $utm_header_image,
  ));

  $vars['header_image_link'] = $header_image_link;
  $vars['newsletter_a_title'] = $newsletter_a_title;
  $vars['newsletter_a_blurb'] = $newsletter_a_blurb;
  $vars['large_a_cards'] = $large_a_cards;
  $vars['ymal_a_cards'] = $ymal_a_cards;
}

function simple_newsletter_preprocess_simple_newsletter_daily_plain_text(&$vars) {
  $nl_node = $vars['node'];

  $vars['blurb'] = strip_tags($nl_node->body[LANGUAGE_NONE][0]['value']);

  $nodes_markup = '';
  foreach ($nl_node->field_article_card[LANGUAGE_NONE] as $card) {
    $node = node_load($card['target_id']);
    $url = url('node/' . $node->nid, array('absolute' => TRUE));
    $link_markup = l($url, $url);
    $nodes_markup .= $node->title . '<br/>' . $link_markup . '<br/><br/>';
  }
  $vars['nodes_markup'] = $nodes_markup;
}

function simple_newsletter_preprocess_simple_newsletter_dedicated(&$vars) {
  $nl_node = $vars['node'];
  $send_date = date('Y-m-d', $nl_node->field_send_date[LANGUAGE_NONE][0]['value']);

  $cta = $nl_node->field_label_link[LANGUAGE_NONE][0];

  $lead_in_uri = $nl_node->field_header_image[LANGUAGE_NONE][0]['uri'];
  if (!empty($lead_in_uri)) {
    // TODO: Image style to be optimized
    $lead_in_image_src = image_style_url('article-852', $lead_in_uri);
    $lead_in_image = theme('image', array('path' => $lead_in_image_src));
    $vars['lead_in_image_link'] = l($lead_in_image, $cta['url'], array('html' => TRUE));
  }

  $vars['blurb'] = strip_tags($nl_node->body[LANGUAGE_NONE][0]['value']);

  $utm_header_image = simple_newsletter_utm_generate(0, $type = 'header_image', $send_date, '', 'dedicated');
  $vars['header'] = theme('simple_newsletter_header', array(
    'nl_node' => $nl_node,
    'utm_header_image' => $utm_header_image,
  ));
  $vars['footer'] = theme('simple_newsletter_footer');

  $vars['title'] = $nl_node->field_title_body[LANGUAGE_NONE][0]['safe_value'];
  $vars['body'] = $nl_node->field_newsletter_body[LANGUAGE_NONE][0]['safe_value'];

  $cta_style = strtolower($nl_node->field_button_style[LANGUAGE_NONE][0]['value']);
  $vars['cta'] = l($cta['title'], $cta['url'], array('attributes' => array('class' => $cta_style)));
}

function simple_newsletter_utm_generate($index = 0, $type = 'image', $date = '', $template_name = '', $campaign_name = 'daily_newsletter'){
  $utm_content = '';
  if (empty($date)){
    $date = date('Y-m-d');
  }
  if ($type === 'header_image') {
    $utm_content = $type;
  }
  else if ($type === 'native_ad') {
    $utm_content = $index;
  }
  else {
    $utm_content = 'story' . ($index + 1) . '_' . $type;
  }
  $utm_campaign = $campaign_name . '_' . $date;
  if ($template_name) {
    $utm_campaign .= '_' . $template_name;
  }
  $utm_params = array(
    'utm_source' => 'Sailthru',
    'utm_medium' => 'email',
    'utm_content' => $utm_content,
    'utm_campaign' => $utm_campaign,
  );
  return $utm_params;
}

function simple_newsletter_css_inline($html, $css = NULL) {
  require_once(dirname(__FILE__) . '/vendor/autoload.php');
  $csstoInlineStyle = new TijsVerkoyen\CssToInlineStyles\CssToInlineStyles();
  $inline_content = $csstoInlineStyle->convert($html, $css);
  return $inline_content;
}

/*
 * Function returning the HTML content of a Newsletter Node to be sent to Sailthru
 */
function simple_newsletter_content_html($template_name, $node, $options = array()) {
  $content = theme('simple_newsletter_' . $template_name, array('node' => $node));
  $css_path = drupal_get_path('module', 'simple_newsletter') . '/theme/css/' . $template_name . '.css';
  $css = file_get_contents($css_path);

  // Ability to switch template preview
  // if (!empty($options['show_template_preview_switch'])) {
  //   $available_templates = array(
  //     'Version A' => 'daily_a',
  //     'Version B' => 'daily_b',
  //   );
  //   $switch_template_preview_markup = '<div class="template-preview-switcher">';
  //   foreach ($available_templates as $template_title => $template_id) {
  //     $switch_template_preview_markup .= l($template_title, '/newsletter/preview/' . $template_id . '/' . $node->nid);
  //   }
  //   $switch_template_preview_markup .= '</div>';
  //   $content = str_replace('</body>', $switch_template_preview_markup . '</body>', $content);
  // }

  $content = simple_newsletter_css_inline($content, $css);
  return $content;
}

/*
 * Callback function called by the Preview Page
 */
function simple_newsletter_callback_newsletter_template_preview($template_name = 'daily_a', $nid = NULL) {
  $node = NULL;
  if (empty($nid)) {
    $node = simple_newsletter_select_latest_nl();
  } else {
    $node = node_load($nid);
  }

  $options = array();
  if ($template_name !== 'dedicated') {
    $options['show_template_preview_switch'] = TRUE;
  }
  $content = simple_newsletter_content_html($template_name, $node, $options);
  if (!is_string($content)) {
    $content = print_r($content, TRUE);
  }
  drupal_deliver_page($content, 'simple_newsletter_deliver_page');
}

/**
 * default_delivery_callback for preview.
 *
 * @param $fragment
 */
function simple_newsletter_deliver_page($fragment) {
  echo $fragment;
}

/*
 * Helper function for devel only. Kill before prod.
 */
function simple_newsletter_select_latest_nl() {
  $query = db_select('node', 'n')
    ->fields('n', array('nid'))
    ->condition('n.type', 'newsletter')
    ->orderBy('n.created', 'DESC')
    ->range(0, 1)
    ->execute();
  $result = $query->fetchColumn();

  return node_load($result);
}

/*
 * Callback to return json encoded node.
 */
function simple_newsletter_node_json($nid = NULL) {
  $json_node = '';
  if($nid){
    $node = node_load($nid);
    $json_node = drupal_json_output($node);
  }
}
