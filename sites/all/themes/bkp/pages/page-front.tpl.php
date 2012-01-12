

<?php
require_once "header-top.tpl.php";
?>
<div id="wrapper">
    <!--Header Start-->
    <?php
    require_once "header.tpl.php";
    ?>


    <!-- Inner Page Header -->
    <!--            <div id="in_header" class="product_hd">
                    <div class="dis">
                        <h1>Products</h1>
                        <p>Stunning Antique gold with colours AD stones on beautifully designed necklace set with matching tika and earrings.</p>
                    </div>
                </div>-->

    <!--Content Start-->

<script type="text/javascript">

function slideSwitch() {
    var $active = $('#slideshow IMG.active');

    if ( $active.length == 0 ) $active = $('#slideshow IMG:last');

    // use this to pull the images in the order they appear in the markup
    var $next =  $active.next().length ? $active.next()
        : $('#slideshow IMG:first');

    // uncomment the 3 lines below to pull the images in random order
    
    // var $sibs  = $active.siblings();
    // var rndNum = Math.floor(Math.random() * $sibs.length );
    // var $next  = $( $sibs[ rndNum ] );


    $active.addClass('last-active');

    $next.css({opacity: 0.0})
        .addClass('active')
        .animate({opacity: 1.0}, 1000, function() {
            $active.removeClass('active last-active');
        });
}

$(function() {
    setInterval( "slideSwitch()", 5000 );
});

</script>
    
    <div id="content_wrapper">
         <?php if ($show_messages && $messages): print $messages;
            endif; ?>
            <?php print $help; ?>
        <!-- Visula Slider -->
       <div id="slideshow">
    <img src="<?php print $front_page . $directory; ?>/images/slider_1.png" alt="slider" class="active" />
    <img src="<?php print $front_page . $directory; ?>/images/slider_2.png" alt="slider" />
     <img src="<?php print $front_page . $directory; ?>/images/slider_1.png" alt="slider" />
      <img src="<?php print $front_page . $directory; ?>/images/slider_2.png" alt="slider" />
    </div>

        <!-- Quick Links -->
        <div class="quicklinks">
            <a href="<?php print $front_page;?>taxonomy/term/3"><img src="<?php print $front_page . $directory; ?>/images/exclusive_collection_banner.png" alt="BKP collection" /></a>
            <a href="<?php print $front_page;?>taxonomy/term/4"><img src="<?php print $front_page . $directory; ?>/images/joinus_banner.png" alt="BKP Special Offers" /></a>
            <a href="<?php print $front_page;?>events" class="last"><img src="<?php print $front_page . $directory; ?>/images/specialoffer_banner.png" alt="BKP Events" /></a>
            <div class="clear"></div>
        </div>



        <div class="clear"></div>
    </div>


    
    <!--Content End-->



    <!--Footer Start-->
    <?php
    require_once 'footer.tpl.php';
    ?>
    <!--Footer End-->

</div>

<?php
require_once "footer-bottom.tpl.php";
?>