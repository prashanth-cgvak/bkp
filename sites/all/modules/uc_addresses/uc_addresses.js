// $Id: uc_addresses.js,v 1.5 2009/12/17 01:23:51 freixas Exp $

/**
 * Set the select box change behavior for the country selector
 */
Drupal.behaviors.ucAddressesCountrySelect = function(context) {
  if (Drupal.settings.uc_address_default) {
    $('select[id$=-country]:not(.ucAddressCountrySelect-processed)', context).each(
      function() {
        uc_update_zone_select(this.id, Drupal.settings.uc_address_default.zone);
      }
    );
  }
}
