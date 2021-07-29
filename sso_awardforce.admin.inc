<?php

function sso_awardforce_settings_form($form, $form_state)
{
    $form['sso_awardforce_installation_domain'] = array(
        '#type' => 'textfield',
        '#title' => t('Award Force URL'),
        '#default_value' => variable_get('sso_awardforce_installation_domain', ''),
        '#required' => TRUE,
        '#size' => 30,
        '#description' => t('Enter you Award Force URL (without trailing slash). For example: myawards.awardsplatform.com'),
        '#field_prefix' => 'https://',
    );

    $form['sso_awardforce_api_key'] = array(
        '#type' => 'textfield',
        '#title' => t('API Key'),
        '#default_value' => variable_get('sso_awardforce_api_key', ''),
        '#required' => TRUE,
        '#size' => 58,
        '#description' => t('An Awards Manager can generate this key in Award Force (Settings > Developers > API keys)'),
    );

    $form = system_settings_form($form);

    return $form;
}