<?php
// $Id: page.tpl.php gobal Exp $
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN"   "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
    <head>
        <?php print $head ?>
        <title><?php print $head_title ?></title>
        <?php print $styles ?>
        <?php print $scripts ?>

        <script type="text/javascript"> 
      
            // initialise plugins
            jQuery(function(){
                jQuery('#main_nav ul.menu').addClass('sf-menu');
                jQuery('ul.sf-menu').superfish();
                jQuery('.sitemap ul.menu').removeClass('menu');
                jQuery('.sitemap ul').addClass('links');
            });
       
        </script> 

<script>
$(document).ready(function(){
	ajaxCartUpdateBlockCart();
});
</script>

    </head>
    <body>