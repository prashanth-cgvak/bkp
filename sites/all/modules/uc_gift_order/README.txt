UC Gift Order
=========================
This module lets customers mark an order as a gift during checkout and write a
small message to go along with it.

Features
=========================
1) Creates a new checkout pane that let's users mark an order as a gift and
   write a message
2) If the order is a gift, it is highlighted on the "view order" admin screen.
3) Admin's can create / edit gift messages on the "edit order" admin screen.
4) Admin's can print a modified invoice that shows the gift message, but no
   price information. This would be used to print and include when shipping the
   gift. (See Installation Instructions below for instructions on getting this
   to work).
5) The names and addresses of the recipients of gift orders can be browsed and
   downloaded via Store Administration -> Customers -> Gift Recipients

Installation Instructions
=========================
1) Download the module.
2) Untar it into sites/all/modules and enabble it in Drupal.
3) Copy uc_order-gift_message.tpl.php into into ubercart/uc_order/templates directory
   or into your theme directory if you are custom-theming your order invoices.
   custom theming instructions: http://www.ubercart.org/docs/developer/17385/changing_invoice_templates_ubercart_23_and_later
5) Create a gift order, go to the "Invoice" tab on the order admin screen, and
   click on the "Gift message" sub-tab.  You can edit uc_order-gift_message.tpl.php and
   theme it as needed.

Credits
=========================
Originally develoved by xerbutter and posted in this thread:
http://www.ubercart.org/forum/development/2122/gift_message_checkout_new_module_attached
