<?php

class AwardForceSSO {

  private $api;

  private $installationDomain;

  public function __construct(AwardForceAPI $api) {
    $this->api = $api;
    $this->installationDomain = variable_get('sso_awardforce_installation_domain', '');
  }

  /**
   * Requests an Award Force authentication token and redirects the user to the
   * awards homepage.
   *
   */
  public function sso() {
    global $user;
    $slug = $this->getSlug($user);
    $token = $this->requestAuthToken($slug);
    drupal_goto($this->installationDomain. "/login",
      array(
        'external' => TRUE,
        'query' => array('token' => $token),
      )
    );
    exit;
  }

  /**
   * Returns the Award Force slug of a user.
   *
   *
   * @return mixed
   */
  private function getSlug($user) {
    if (!empty($user->data['award_force_slug'])) {
      return $user->data['award_force_slug'];
    }
    $slug = $this->requestSlug($user);
    $user->data['award_force_slug'] = $slug;
    user_save($user);

    return $slug;
  }

  /**
   * Sends a POST request to the Award Force API to obtain the user's slug.
   *
   *
   * @return mixed
   */
  private function requestSlug($user) {
    $words = explode( ' ', $user->name);
    $first_name = array_shift( $words);
    $last_name = implode( ' ', $words);
    $response = $this->api->post('/user', [
      'form_params' => [
        'email'     => $user->mail,
        'firstName' => $first_name ?: 'First',
        'lastName'  => $last_name ?: 'Last'
      ],
    ]);

    return $response->slug;
  }

  /**
   * Sends a GET request to the Award Force API to obtain an authentication
   * token for the user with the given slug.
   *
   * @param $slug
   *
   * @return mixed
   */
  private function requestAuthToken($slug) {
    $response = $this->api->get('/user/' . $slug . '/auth-token');

    return $response->auth_token;
  }
}