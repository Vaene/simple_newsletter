require 'sass'
require 'compass'

# Require any additional compass plugins here.
require 'survivalkit'
require 'breakpoint'
require 'sassy-buttons'
require 'singularitygs'
#require 'bootstrap-sass'

# Set this to the root of your project when deployed:
http_path = "/"
css_dir = (environment == :production) ? "css/live" : "css"
sass_dir = "sass"
fonts_dir = "fonts"
images_dir = "images"
javascripts_dir = "js"
add_import_path "../../../../../themes/greatist2x/sass"
# add_import_path "/sites/all/themes/greatist2x/sass"

# Also assuming this theme is in sites/*/themes/THEMENAME, you can add the partials
# included with a module by uncommenting and modifying one of the lines below:
#add_import_path "../../../default/modules/FOO"
#add_import_path "../../../all/modules/FOO"
#add_import_path "../../../../modules/FOO"

# You can select your preferred output style here (can be overridden via the command line):
output_style = (environment == :production) ? :compressed : :expanded

# To enable relative paths to assets via compass helper functions. Uncomment:
relative_assets = true

# To disable debugging comments that display the original location of your selectors. Uncomment:
line_comments = (environment == :production) ? false : true

# If you prefer the indented syntax, you might want to regenerate this
# project again passing --syntax sass, or you can uncomment this:
preferred_syntax = :sass
# and then run:
# sass-convert -R --from scss --to sass sass scss && rm -rf sass && mv scss sass

# Add cache buster
asset_cache_buster :none

if RUBY_VERSION =~ /2.0/
  Encoding.default_external = Encoding::UTF_8
  Encoding.default_internal = Encoding::UTF_8
end

::Sass::Script::Number.precision = [10, ::Sass::Script::Number.precision].max
