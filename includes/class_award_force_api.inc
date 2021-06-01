<?php

use GuzzleHttp\Client;

class AwardForceAPI
{
    private $apiUrl = 'https://api.cr4ce.com';

    private $apiKey;

    public function __construct()
    {
        $this->apiKey = variable_get('sso_awardforce_api_key', '');
    }

    /**
     * Sends a GET request to the Award Force API.
     *
     * @param $uri
     * @param array $options
     *
     * @return \Psr\Http\Message\ResponseInterface
     */
    public function get($uri, $options = [])
    {
        try {
            $response = $this->getClient()->get($uri, $options);
            return json_decode($response->getBody()->getContents());
        } catch (Exception $e) {
            $this->handleException($e);
        }
    }

    /**
     * Sends a POST request to the Award Force API.
     *
     * @param $uri
     * @param array $options
     *
     * @return \Psr\Http\Message\ResponseInterface
     */
    public function post($uri, $options = [])
    {
        try {
            $response = $this->getClient()->post($uri, $options);
            return json_decode($response->getBody()->getContents());
        } catch (Exception $e) {
            $this->handleException($e);
        }
    }

    /**
     * Retrieves the API access token from the database. If not available,
     * requests it to the API.
     *
     * @return string
     */
    private function getAccessToken()
    {
        if ($token = variable_get('sso_awardforce_access_token', '')) {
            return $token;
        }

        $token = $this->requestAccessToken();

        variable_set('sso_awardforce_access_token', '');

        return $token;
    }

    /**
     * Requests an access token to the Award Force API.
     *
     * @return string
     */
    private function requestAccessToken()
    {
        try {
            $client = new Client([
                'base_uri' => $this->apiUrl,
                'headers' => [
                    'Accept' => 'application/vnd.Award Force.v1.0+json',
                    'Authorization' => 'Basic ' . $this->apiKey,
                ],
            ]);

            $response = $client->get('/access-token');
            return $response->getBody()->getContents();
        } catch (Exception $e) {
            $this->handleException($e);
        }
    }

    /**
     * Returns an instance of an HTTP Client.
     *
     * @return Client
     */
    private function getClient()
    {
        return new Client([
            'base_uri' => $this->apiUrl,
            'headers' => [
                'Accept' => 'application/vnd.Award Force.v1.0+json',
                'Authorization' => 'Basic ' . $this->getAccessToken(),
            ],
        ]);
    }

    /**
     * Logs any potential errors, clears the API access token from the database
     * and displays an error message to the user.
     *
     * @param Exception $e
     */
    private function handleException(Exception $e)
    {
        variable_del('sso_awardforce_access_token');

        watchdog('Award Force API', $e->getMessage(), array(), WATCHDOG_ERROR);

        exit(t('An error has occurred. Please try again, and if the problem persists, contact the system administrator.'));

    }
}
