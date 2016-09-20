<?php

/**
 * Implements hook_mail_alter()
 * @param string $message
 */
function simple_newsletter_mail_alter(&$message) {
  $headers = array(
    'MIME-Version' => '1.0',
    'Content-Type' => 'text/html; charset=iso-8859-1; format=flowed',
    'Content-Transfer-Encoding' => '8Bit',
    'X-Mailer' => 'Drupal',
  );
  foreach ($headers as $key => $value) {
    $message['headers'][$key] = $value;
  }
}

/**
 * Implements hook_mail()
 * @param string $key
 * @param string $message
 * @param unknown_type $params
 */
function simple_newsletter_mail($key, &$message, $params) {

  //Language Selection
  $options = array(
    'langcode' => $message['language']->language,
  );
  switch($key) {
    case "daily_test_email" :
      $message['subject'] = isset($params['subject']) ? $params['subject'] : $message['subject'] = t('Test Daily Email @site-name', array('@site-name' => variable_get('site_name', 'Greatist')), $options);
      $message['body'][] = isset($params['body']) ? $params['body'] : NULL;
      if (isset($params['headers']) && is_array($params['headers'])) {
        $message['headers'] += $params['headers'];
      }
      break;
  }
}

function simple_newsletter_mail_send(array $values = array()) {

  dpm($values,'values');
  $module = $values['module'];
  $key = $values['key'];
  $to = $values['to'];
  $from = $values['from'];
  $language = isset($values['lang']) ? $values['lang'] : language_default();
  $params = array(
    'subject' => $values['subject'],
    'body' => $values['body'],
  );
  dpm($values['body']);
  if(array_key_exists('headers', $values)) {
    $params['headers'] = $values['headers']; //Assumed as an array
  }
  $send = TRUE;
  $mail = drupal_mail($module, $key, $to, $language, $params, $from, $send);
  dpm($mail,'mail');
  if($mail['result']) {
    return TRUE;
  } else {
    $error_msg = 'Failed to send the email in simple_newsletter Module';
    watchdog('simple_newsletter-test-email', $error_msg, array(), WATCHDOG_ALERT);
    return FALSE;
  }
}


//class Gr8istNewsletterMailSystem extends DefaultMailSystem {
//  /**
//   * Concatenate and wrap the e-mail body for plain-text mails.
//   *
//   * @param $message
//   *   A message array, as described in hook_mail_alter().
//   *
//   * @return
//   *   The formatted $message.
//   */
//  public function format(array $message) {
//    $message['body'] = implode("\n\n", $message['body']);
//    return $message;
//  }
//
//  /**
//   * Send an e-mail message, using Drupal variables and default settings.
//   *
//   * @see http://php.net/manual/en/function.mail.php
//   * @see drupal_mail()
//   *
//   * @param $message
//   *   A message array, as described in hook_mail_alter().
//   * @return
//   *   TRUE if the mail was successfully accepted, otherwise FALSE.
//   */
//  public function mail(array $message) {
//    $mimeheaders = array();
//    foreach ($message['headers'] as $name => $value) {
//      $mimeheaders[] = $name . ': ' . mime_header_encode($value);
//    }
//    $line_endings = variable_get('mail_line_endings', MAIL_LINE_ENDINGS);
//    return mail(
//      $message['to'],
//      mime_header_encode($message['subject']),
//      // Note: e-mail uses CRLF for line-endings. PHP's API requires LF
//      // on Unix and CRLF on Windows. Drupal automatically guesses the
//      // line-ending format appropriate for your system. If you need to
//      // override this, adjust $conf['mail_line_endings'] in settings.php.
//      preg_replace('@\r?\n@', $line_endings, $message['body']),
//      // For headers, PHP's API suggests that we use CRLF normally,
//      // but some MTAs incorrectly replace LF with CRLF. See #234403.
//      join("\n", $mimeheaders)
//    );
//  }
//}


