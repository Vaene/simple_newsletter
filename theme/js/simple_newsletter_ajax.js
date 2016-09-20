/*
 * @file
 * JavaScript for newsletter autocomplete.
 *
 */

(function ($) {

  // Newsletter Ajax workaround for IEF form AJAX difficulties
  $(document).ready(function () {

    // Loop through inputs of a certain name to get the #
    var rowcount = $("#ief-entity-table-edit-field-article-card-und-entities tr").filter(function() {
      return $(this).find('[name^="field_article_card"]').length;
    }).length;
    for (i=0;i<rowcount;i++){
      var inputName = "input[name='field_article_card[und][entities][" + i + "][form][field_content][und][0][target_id]']";
      $(inputName).bind('autocompleteSelect', function (eventObject) {
        // Get id from currentTarget
        var idarr = eventObject.currentTarget.id.split("-");
        var id = idarr[6];
        var str_array = eventObject.target.value.split("(");
        var nid_array = str_array[str_array.length - 1].split(")");
        var nid = nid_array[0];

        $.getJSON("/newsletter/nodejson/" + nid, function (data) {

          // Get title.
          title = data.title;
          if ($.isEmptyObject(data.field_promo_title) === false) {
            title = data.field_promo_title.und[0].value;
          }
          title_id = "#edit-field-article-card-und-entities-" + id + "-form-title";
          $(title_id).val(title);
          // Get Blurb
          var summary = '';

          if ($.isEmptyObject(data.field_newsletter_blurb) === false) {

            summary = data.field_newsletter_blurb.und[0].value;

          } else if ($.isEmptyObject(data.body) === false) {
            summary = data.body.und[0].summary;
          }
          blurb_id = "#edit-field-article-card-und-entities-" + id + "-form-field-blurb-override-und-0-value";
          $(blurb_id).val(summary);

          // Get Image.

          var fid = 0;
          var image_name = '';
          if ($.isEmptyObject(data.field_image_homepage_story) === false){
            fid = data.field_image_homepage_story.und[0].fid;
            image_name = data.field_image_homepage_story.und[0].filename;
          }

          var preview_markup = $(["<div class='media-item' data-fid='" + fid + "' title='" + image_name + "'>",
            "<div class='media-thumbnail'>",
            "<img width='150' height='103' alt='' src='http://greatist.site/sites/default/files/styles/media_thumbnail/public/" + image_name + "' typeof='foaf:Image'>",
            "<div class='label-wrapper'>",
            "<label class='media-filename'>" + image_name + "</label>",
            "</div>",
            "</div>",
            "</div>"
          ].join("\n"));
          image_id = "#edit-field-article-card-und-entities-" + id + "-form-field-image-override-und-0-upload";
          $(image_id).html(preview_markup);

          image_fid = "input[name='field_article_card[und][entities][" + id + "][form][field_image_override][und][0][fid]']";
          $("").val(fid);

        });
      });
    }
  });

})(jQuery);
