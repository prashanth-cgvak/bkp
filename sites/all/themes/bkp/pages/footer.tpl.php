<div id="footer">
    <div id="follow">
        <div class="<?php print $logged_in ? "signup" : "signup-guest"; ?>">
            <h3>Sign up for our newsletter</h3>
            <?php
            print $news_letter;
            ?>
        </div>
        <div class="f_us">
            <h5>Get the latest news, FOLLOW US NOW</h5>
            <a href="http://twitter.com/#!/BKPJewels" target="_blank"><img src="<?php print $front_page . $directory; ?>/images/i_twitter.gif" alt="Twitter" /></a>
            <a href="http://www.facebook.com/pages/BKP-Jewels/124477670940373" target="_blank"><img src="<?php print $front_page . $directory; ?>/images/i_facebook.gif" alt="Twitter" /></a>
            <a href="#"><img src="<?php print $front_page . $directory; ?>/images/i_youtube.gif" alt="Twitter" /></a>
            <a href="#"><img src="<?php print $front_page . $directory; ?>/images/i_blog.gif" alt="Twitter" /></a>
        </div>	
        <div class="clear"></div>
    </div>
    <div class="sitemap">
        <?php
        print $footer_personal;
        ?>
        <?php
        print $footer_pages;
        ?>
        <?php
        print $footer_social;
        ?>
     

        <img src="<?php print $front_page . $directory; ?>/images/geotrust.gif" alt="GeoTrust SSL Certificates" class="right" />
    </div>
    <div class="clear"></div>

    <div class="creditcards"><img src="<?php print $front_page . $directory; ?>/images/cards.gif" alt="we accept" class="right" /></div>
    <div class="powered">
        &copy; 2011 All Rights reserved BKP Jewels. &nbsp; Powered by <a href="http://blueit-services.co.uk/"><img src="<?php print $front_page . $directory; ?>/images/blueit_icon.gif" alt="Blue IT"  vspace="0"/></a> <a href="http://blueit-services.co.uk/">Blue IT</a>
    </div>

    <div class="clear"></div>
</div>