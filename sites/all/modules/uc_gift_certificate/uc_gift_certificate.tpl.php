<?php

/**
 * @file
 * This file is the default email template for when someone receives a gift certificate
 * You have access to the global, gift certificate, and user account token variables.
 * Additional variables:
 *  The gift cert object: $gift_certificate
 *  The recipient user object: $account
 *  The order object, if there is one: $order
 *  The order_product object, if there is one, which is useful if you have other attributes associated with the certificate: $order_product
 *  Is new user flag: $is_new_user
 *  New user password: $new_password
 */
?>
<p>Good news!</p>
<p>You have just received a gift certificate for <strong><?php echo $gift_cert_value; ?></strong> to be used on your next purchase at <?php echo $store_link; ?>.</p>
<p>Message from the sender:</p>
<blockquote><?php echo $gift_cert_message; ?></blockquote>
<?php if ($is_new_user): ?>
<p><strong>An account has been created for you with the following details:</strong></p>
<p>
  <strong>Username:</strong> <?php echo $user; ?><br />
  <strong>Password:</strong> <?php echo $new_password; ?>
</p>
<?php endif; ?>
<p>To use your gift certificate, click on the following link to log in and start shopping:<br />
<?php echo $site_login; ?>
</p>
<p>Happy shopping!</p>
