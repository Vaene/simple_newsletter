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
        <?php print $blurb; ?>
      </div>
      <?php if (!empty($header)): ?>
        <?php print $header;?>
        <div class="separator"></div>
      <?php endif; ?>

      <?php if (!empty($lead_in_image_link)): ?>
        <div class="lead-in-image">
          <?php print $lead_in_image_link; ?>
        </div>
      <?php endif; ?>

      <div class="content">
        <h1 class="title">
          <?php print $title ?>
        </h1>
        <div class="body">
          <?php print $body ?>
        </div>
        <div class="cta">
          <?php print $cta ?>
        </div>
      </div>

      <?php if (!empty($footer)): ?>
        <div class="separator"></div>
        <?php print $footer; ?>
      <?php endif; ?>
    </div>
  </body>
</html>
