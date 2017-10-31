# Award Force - Drupal Single Sign-On (SSO)

For Award Force clients with a website built with Drupal, this module allows your registered users to automatically log in to Award Force with their drupal user account. An Award Force user account is automatically created for them.

## Installation

- Download the [latest release](https://github.com/tectonic/sso-awardforce-drupal/releases) of the Drupal module from this repository.
- Log in to your Drupal site's admin area and install the module `Modules > Install new module`
- Activate it!

## Usage

- Configure the plugin by adding an `API Key` and your `Award Force URL`.
- Add a link in your theme to the following URL: `/awardforce/sso`. Make sure this link is only visible to authenticated users, e.g.:

```
<a href="/awardforce/sso">Award Force</a>
```

- When users click this link the plugin will redirect them to your Award Force account and log them in.
