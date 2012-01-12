

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
            <div id="incontainer">
                <div class="tax-cat">
                    <?php print $content_blocks; ?>
                </div>
                <div class="page_heading">
                    <?php if ($title): print '<h1>' . $title . '</h1>';
                    endif; ?>

                </div>
                <div class="content-wrapper">
                    <?php if ($tabs): print '<ul class="tabs secondary">' . $tabs . '</ul>';
                    endif; ?>

                    <?php if ($tabs2): print '<ul class="tabs secondary">' . $tabs2 . '</ul>';
                    endif; ?>


                    <?php if ($show_messages && $messages): print $messages;
                    endif; ?>
                    <?php print $help; ?>
                    <div class="content">
                        <?php
                        print $content;
                        ?>

                    </div>

                </div>
                <div class="clear"></div>

                <div class="ft_pagination">
                    
                    <?php
                    if($node->type=='product'){
                        print $related_products;
                    }
                    ?>
                </div>

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