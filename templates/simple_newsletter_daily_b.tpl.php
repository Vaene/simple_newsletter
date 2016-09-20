<html>
  <head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  </head>

  <?php if (!empty($style)): ?>
    <?php print $style; ?>
  <?php endif; ?>

  <img src="{beacon_src}" alt="Greatist" class="beacon" />

  <body>
    <div class="wrapper">
      <div class="lead-blurb">
        <?php print $newsletter_b_blurb; ?>
      </div>
      <?php if (!empty($header)): ?>
        <?php print $header;?>
        <div class="separator"></div>
      <?php endif; ?>

      <div class="content">
        <?php if (!empty($cards)): ?>
          <?php print $cards; ?>
        <?php endif; ?>
      </div>

      <?php if (!empty($footer)): ?>
        <div class="separator"></div>
        <?php print $footer; ?>
      <?php endif; ?>
    </div>
  </body>
</html>
