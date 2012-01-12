<div id="header">
    <div id="logo">
        <a href="<?php print $front_page; ?>">
            <img src="<?php print $front_page . $directory; ?>/images/logo.gif" alt="BKP Jewels Logo" />
        </a>
    </div>

    <div id="top_right_block">
        <ul id="topmenu_right">
            <?php
            if ($logged_in) {
                ?>
                <li><a href="<?php print $front_page; ?>user/<?php print $user->uid; ?>">My Account</a></li>
                <li class="line">|</li>
                <li><a href="<?php print $front_page; ?>user/<?php print $user->uid; ?>/orders">My Orders</a></li>
                <?php
            } else {
                ?>
                <li><a href="<?php print $front_page; ?>user">Login</a></li>
                <li class="line">|</li>
                <li><a href="<?php print $front_page; ?>user/register">Create Account</a></li>
                <?php
            }
            ?>
            <li class="line">|</li>
            <li>
                <a href="<?php print $front_page; ?>cart"> <?php print $ajax_cart; ?></a>
            </li>
            <li class="line">|</li>
            <?php
            if ($logged_in) {
                ?>
                <li ><a href="<?php print $front_page; ?>cart/checkout">Checkout</a></li>
                <li class="line">|</li>
                <li class="last"><a href="<?php print $front_page; ?>logout">Logout</a></li>
                <?php
            } else {
                ?>

                <li class="last"><a href="<?php print $front_page; ?>cart/checkout">Checkout</a></li>
                <?php
            }
            ?>
        </ul>

        <div id="search_main">
            <?php
            print $search_block;
            ?>
        </div>
        <div class="clear"></div>
        <div class="offers">
            <?php
            print $message_box;
            ?>
        </div> 

    </div>
    <div class="clear"></div>
</div>

<!-- Main Nav -->
<div id="main_nav">
    <?php print $primary_menu; ?>
    <div class="clear"></div>
</div>