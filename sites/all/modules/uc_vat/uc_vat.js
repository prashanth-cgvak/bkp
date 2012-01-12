// $Id: uc_vat.js,v 1.2 2010/09/17 19:55:34 longwave Exp $

function updateCartPane() {
  var order = serializeOrder();

  if (!!order) {
    $.ajax({
      type: "POST",
      url: Drupal.settings.ucURL.updateCartPane,
      data: 'order=' + Drupal.encodeURIComponent(order),
      dataType: "json",
      success: function(data) {
        if (data.cart) {
          $('#cart-pane div:first').html(data.cart);
        }
      }
    });
  }
}

$(function() {
  var progressBar = new Drupal.progressBar('uc_vat');
  progressBar.setProgress(-1);
  $('#cart-pane div:first').html(progressBar.element);
  updateCartPane();

  $("select[name*=delivery_country], select[name*=billing_country]").change(function() {
    updateCartPane();
  });
});
