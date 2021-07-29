<?php

class AwardForceSSO {

    private $api;
    private $installationDomain;

    public function __construct(AwardForceAPIV2 $api)
    {
        $this->api = $api;
        $this->installationDomain = variable_get('sso_awardforce_installation_domain', '');
    }

    /**
     * Requests an Award Force authentication token and redirects the user to the awards homepage.
     *
     */
    public function sso()
    {
        global $user;

        $slug = $this->getSlug($user);
        $token = $this->requestAuthToken($slug, $user);

        drupal_goto('https://'.$this->installationDomain.'/login', array('external' => true, 'query' => array('token' => $token)));

        exit;
    }

    /**
     * Returns the Award Force slug of a user.
     *
     * @param $user
     * @param bool $forceRequest
     * @return mixed
     */
    private function getSlug($user, $forceRequest = false)
    {
        if (!$forceRequest && !empty($user->data['award_force_slug'])) {
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
     * @param $user
     * @return mixed
     */
    private function requestSlug($user)
    {
        $response = $this->requestSlugByEmail($user->mail);

        if (isset($response->slug)) {
            return $response->slug;
        }

        $response = $this->createUser($user);

        if (!isset($response->slug)) {
            $this->api->handleException(new Exception($response->message ?: 'There was an error creating the user.'));
        }

        return $response->slug;
    }

    private function createUser($user)
    {
        $words = explode(' ', $user->name);

        $firstName = array_shift($words);
        $lastName = implode(' ', $words);

        return $this->api->post('/user', [
            'email' => $user->mail,
            'first_name' => $firstName ?: 'First',
            'last_name' => $lastName ?: 'Last',
            'password' => uniqid(),
        ]);
    }

    private function requestSlugByEmail($email)
    {
        return $this->api->get("user/" . $email);
    }

    /**
     * Sends a GET request to the Award Force API to obtain an authentication token for the user with the given slug.
     *
     * @param $slug
     * @return mixed
     */
    private function requestAuthToken($slug, $user)
    {
        if ($token = $this->sendAuthTokenRequest($slug)->auth_token) {
            return $token;
        }

        $slug = $this->getSlug($user, true);
        $retries = 5;

        while ($retries > 0) {
            if ($response = $this->sendAuthTokenRequest($slug)) {
                if ($token = $response->auth_token) {
                    return $token;
                }
                $this->api->handleException(new Exception($response->message));
            }
            sleep(1);
            $retries--;
        }

        if (!$token) {
            $this->api->handleException(new Exception('There was an error requesting a token from Award Force'));
        }

        return $token;
    }

    private function sendAuthTokenRequest($slug)
    {
        return $this->api->get('/user/' . $slug . '/auth-token');
    }
}
