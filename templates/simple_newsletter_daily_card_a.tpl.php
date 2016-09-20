<?php
/**
 * @file
 * large-a-card template.
 * TODO: l() all the links
 */
?>

<tr>
  <td class="td-node" style="padding: 0;">
    <div class="node-wrapper article">
      <div class="media-content" style="margin-bottom: 1em; width: 100%;">
        <a href="<?php print $large_a_card_image_link; ?>" style="color: #f73a6f; text-decoration: none;">
          <img src="<?php print $large_a_card_img_src; ?>" class="feature-img" alt="<?php print $large_a_title; ?>" style="width: 100%;">
        </a>
      </div>

      <?php if ($partner): ?>
        <div class="partner" style="color: #8d30a0; font-weight: bold; margin-bottom: 0.5em;">
          <?php print $partner; ?>
        </div>
      <?php endif; ?>

      <div class="text-content" style="padding-bottom: 16px;">
        <?php if ($large_a_title): ?>
          <div class="title" style="margin-bottom: 0.5em; overflow: hidden;">
            <a href="<?php print $large_a_card_title_link; ?>" class="title-link" style="color: #f73a6f; text-decoration: none;">
              <h2 style="font-weight: normal; line-height: 140%; margin: 0; text-rendering: optimizeLegibility; font-family: helvetica; font-size: 1.5em; color: #333 !important;">
                <?php print $large_a_title; ?>
              </h2>
            </a>
          </div>
          <div class="desc" style="font-size: 1.2em; font-weight: 100; line-height: 140%;">
            <?php print $large_a_summary; ?></div>
          <div class="read-more" style="font-size: 1.2em; margin-bottom: 2em; margin-top: 0.5em; white-space: nowrap;">
            <a href="<?php print $large_a_card_cta_link; ?>" style="color: #f73a6f; text-decoration: none;">
              <span><?php print $large_a_card_cta_label; ?></span>
              <i style="font-style: normal;">›</i>
            </a>
          </div>
        <?php endif; ?>
      </div>
    </div>
  </td>
</tr>
