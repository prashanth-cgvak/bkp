<h2><img src="http://<?php print $_SERVER['SERVER_NAME']; ?>/redcherry/sites/all/themes/redcherry/images/logo.gif" /></h2>
<table cellspacing="0" cellpadding="0" border="0" class="table-product">
    <tbody>
        <tr>
            <td class="padd_1">



                <table cellspacing="0" cellpadding="0" border="0">
                    <tbody><tr valign="top">
                            <td width="51%">
                                <div id="fancy">
                                    <table cellspacing="0" cellpadding="2" border="0" align="right">
                                        <tbody>
                                            <tr valign="top">
                                                <td align="center" class="smallText">
                                                    <?php
                                                    foreach ($node->field_image_cache as $value) {
                                                        $filepath = $value['filepath'];
                                                        $newpath = str_replace(' ', '--', $filepath);
                                                        if (!copy($filepath, $newpath)) {
                                                            // echo "failed to copy ...\n check directory permissions";
                                                        }
                                                        print theme('imagecache', 'product_list', $newpath, $value['alt']);
                                                    }
                                                    ?>

                                                </td>

                                            </tr>
                                        </tbody>
                                    </table>
                                </div>
                                <!-- EOF Simple multi image addon -->
                            </td>
                            <td width="49%">
                                <table cellspacing="0" cellpadding="0" border="0">
                                    <tbody>
                                        <tr>
                                            <td width="40%" valign="middle" height="30" align="left" style="font-family:Arial; font-size:12px; font-weight:bold;">Product Price:</td>
                                            <td width="6%" height="30" style="font-family:Arial; font-size:12px; font-weight:bold;">&nbsp;</td>
                                            <td width="54%" style="font-family:Arial; font-size:12px; font-weight:bold;">
                                                <span class="productSpecialPrice">
                                                    <?php print $node->content['sell_price']['#value']; ?>
                                                </span></td>
                                        </tr>
                                        <?php
                                                    if (isset($node->content['field_product_discount_price']['#children'])) {
                                        ?>
                                                        <tr>
                                                            <td width="40%" valign="middle" height="30" align="left" style="font-family:Arial; font-size:12px; font-weight:bold;">Discounted Price:</td>
                                                            <td width="6%" height="30" style="font-family:Arial; font-size:12px; font-weight:bold;">&nbsp;</td>
                                                            <td width="54%" style="font-family:Arial; font-size:12px; font-weight:bold;">
                                                                <span class="productSpecialPrice">
                                                    <?php print $node->content['field_product_discount_price']['#children']; ?>
                                                    </span></td>
                                            </tr>
                                        <?php
                                                    }
                                        ?>

                                                    <tr>
                                                        <td valign="middle" height="30" align="left" style="font-family:Arial; font-size:12px; font-weight:bold;">Model Number:</td>
                                                        <td height="30" style="font-family:Arial; font-size:12px; font-weight:bold;">&nbsp;</td>
                                                        <td style="font-family:Arial; font-size:12px; font-weight:bold;">
                                                <?php print $node->model; ?>
                                                </td>
                                            </tr>



                                            <tr>
                                                <td height="30" style="font-family:Arial; font-size:12px; font-weight:bold; " colspan="3">


                                                    <table cellspacing="0" cellpadding="0" border="0" width="100%" style="font-family:Arial; font-size:12px; font-weight:bold; ">
                                                        <tbody>
    <!--                                                        <tr>
                                                            <td valign="middle" height="40" align="left" style=" padding:10px 0px 0px 0px; text-transform:uppercase; font-size:12px; color:black; font-family:Arial, Helvetica, sans-serif;" colspan="3">
                                                                <a href="tell_a_friend.php?products_id=263&amp;osCsid=ijjc9pgt1aum5o78624kp61ja6" style="color:#666; text-decoration:none;">Refer a Friend</a></td>
                                                        </tr>-->
                                                            <tr>
                                                                <td valign="top" height="30" align="left" style="font-size:12px; font-weight:normal; color:black; font-family:Arial, Helvetica, sans-serif;" colspan="3">
                                    		Product Description:<br>
                                                                <?php print $node->content['body']['#value']; ?>
                                                            </td>
                                                        </tr>

                                                    </tbody></table></td>
                                        </tr>
                                    </tbody></table></td>
                        </tr>
                    </tbody></table>

                <div style="clear:both;"></div>
            </td></tr></tbody></table>
