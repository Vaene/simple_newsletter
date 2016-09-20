<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
  <title><?php print $newsletter_a_title; ?> </title>
</head>

<body topmargin="0" rightmargin="0" leftmargin="0" style="background: #fff; color: #000; font-family: 'CircularPro Book',Futura,Helvetica,Arial,sans-serif;">

<img src="{beacon_src}" alt="Greatist" class="beacon" />

<div class="body-email" style="background-color: #fff; font-family: Helvetica,sans-serif; margin: 0 auto; padding: 0; width: initial;">
  <div class="main-wrap" style="font-size: 14px; margin: 0 auto; max-width: 644px;">
    <div class="lead-blurb" style="color: #909090; margin-bottom: 0.5em; margin-top: 1em;">
      <?php print $newsletter_a_blurb; ?></div>
    <div class="header" style="background: #000; height: 3em; padding: 1em; text-align: center;">
      <a href="<?php print $header_image_link; ?>" class="logo" style="color: #f73a6f; text-decoration: none; display: block;">
        <img src="http://media.sailthru.com/35b/1jz/c/g/567202b435964.png" alt="Greatist" height="40" style="display: block; margin: auto;"></a>
    </div>
    <table style="border-spacing: 0px;">
      <tbody>
        <?php print $large_a_cards; ?>
      </tbody>
    </table>

    <div class="others-node">
      <h3 style="border-top: 3px solid #c5c5c5; color: #c5c5c5; font-size: 1.8em; font-weight: 500; padding-top: 1em;">You Might Also Like</h3>
      <table style="border-spacing: 0px;">
        <tbody>
          <?php print $ymal_a_cards; ?>
          <tr class="separator" style="height: 1em;"></tr>
        </tbody>
      </table>
    </div>

    <div class="footer-wrapper" style="border-top: 3px solid #000; font-family: Helvetica; margin: auto; margin-top: 1em; text-align: center; text-decoration: none; width: 100%;">
      <div class="social-links" style="padding: 1em;">
        <a href="https://www.facebook.com/greatist" style="color: #f73a6f; text-decoration: none; -moz-border-radius: 16px; -webkit-border-radius: 16px; background: #000; border-radius: 16px; display: inline-block; margin: 0 1em; padding: 8px;">
          <img src="http://media.sailthru.com/35b/1jz/c/7/5665f0bf29ee1.png" width="16" height="16" alt="Greatist on Facebook" style="display: block; height: auto; width: 16px;"></a>
        <a href="http://www.pinterest.com/greatist/" style="color: #f73a6f; text-decoration: none; -moz-border-radius: 16px; -webkit-border-radius: 16px; background: #000; border-radius: 16px; display: inline-block; margin: 0 1em; padding: 8px;">
          <img src="http://media.sailthru.com/35b/1jz/c/7/5665f0bf6b317.png" width="16" height="16" alt="pinterest.png" style="display: block; height: auto; width: 16px;"></a>
        <a href="https://twitter.com/greatist" style="color: #f73a6f; text-decoration: none; -moz-border-radius: 16px; -webkit-border-radius: 16px; background: #000; border-radius: 16px; display: inline-block; margin: 0 1em; padding: 8px;">
          <img src="http://media.sailthru.com/35b/1jz/c/7/5665f0bfdbd66.png" width="16" height="16" alt="Greatist on Twitter" style="display: block; height: auto; width: 16px;"></a>
        <a href="http://www.instagram.com/greatist/" style="color: #f73a6f; text-decoration: none; -moz-border-radius: 16px; -webkit-border-radius: 16px; background: #000; border-radius: 16px; display: inline-block; margin: 0 1em; padding: 8px;">
          <img src="http://media.sailthru.com/35b/1jz/c/7/5665f71a3acad.png" width="16" height="16" alt="Greatist on Instagram" style="display: block; height: auto; width: 16px;"></a>
      </div>

      <div class="footer">
        <div class="unsub" style="line-height: 2rem; margin-top: 1em;"><a href="{optout_confirm_url}" style="color: #000; text-decoration: none;">UNSUBSCRIBE</a></div>
        <div class="copyright" style="line-height: 2rem; font-weight: bold;">© GREATIST <?php echo date("Y"); ?></div>
        <div class="address" style="line-height: 2rem; color: #c5c5c5; font-size: 0.6rem; margin-bottom: 0.5rem; text-align: center;"><span style="font-weight: bold;">Greatist Inc.</span> 30 Vandam St, Floor 3, New York, NY 10013</div>
        <div class="view-in-browser" style="line-height: 2rem;"><a href="{view_url}" style="color: #000; text-decoration: none; display: block; font-size: 10px; margin: auto; text-align: center; width: 100%;">View in Browser</a></div>
      </div>
    </div>
  </div>
</div>
</body>
</html>
