uc_vat installation notes
=========================

If you entered your product prices inclusive of VAT before installing
uc_vat, product prices will have VAT added again once you set up a tax
rule, as they must be stored exclusive of VAT in the database. To reset
your prices so they are stored exclusive of VAT, carry out the following
steps:

1. Make a backup of your database, just in case anything goes wrong!

2. Go to /admin/store/settings/taxes/vat and disable "Keep VAT inclusive
   prices the same after a rate change".

3. Edit your tax rule and set the rate to 0%.

4. Go back to /admin/store/settings/taxes/vat and enable "Keep VAT
   inclusive prices the same after a rate change".

5. Edit your tax rule again and set the rate to the correct percentage.
   When you save the tax rule, all product and attribute prices will be
   recalculated so they are now stored exclusive of VAT.



Displaying VAT inclusive prices in node templates
=================================================

Printing $node->sell_price will only show the price before it is altered
(i.e. excluding VAT), you must call uc_price() to run VAT calculations
in a node template and display a price:

$context = array(
  'revision' => 'themed',
  'type' => 'product',
  'class' => array('product', 'sell_price'),
  'subject' => array('node' => $node),
);

print uc_price($node->sell_price, $context);
