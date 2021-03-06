<?php

/**
 *
 * @param $form
 * @param $form_state
 *
 * @return mixed
 */

function tripal_curator_cv_usage_form($form, &$form_state) {

  $markup = "<h3>Tripal Curator CV usage</h3>
   <p>
This page is for managing your properties by controlled vocabulary.  Some modules previously used <strong>custom </strong> CVs that are not meaningful outside of the site.  Here, you can target properties using this CVs.
</p>
";

  $form['welcome'] = [
    '#markup' => $markup,
  ];

  $cv_usage = tripal_curator_get_prop_by_cv_count();

  foreach ($cv_usage as $prop_table => $cvs) {

    $total_count = 0;
    $header = [
      'CV',
      'Unique terms',
      //   'Total entities',
      //  "Edit usage",
    ];
    $rows = [];


    foreach ($cvs as $cv) {
      $cv_name = $cv->name;
      $cv_id = $cv->cv_id;
      $count = $cv->count;
      $total_count += $count;

      //$edit_link = '<a href = "CV_usage/' . $cv_id . '">'.  . ' </a>';
      $edit_link = l($cv_name, 'admin/tripal/extension/tripal_curator/CV_usage/' . $cv_id);
      $rows[] = [
        'CV' => $edit_link,
        'Unique terms used' => $count,
        //  'Total entities' => "not implemented yet",// $entity_count,
      ];
    }
    //pagination settings
    $per_page = 10;
    // Initialize the pager
    $current_page = pager_default_initialize(count($rows), $per_page);
    // Split your list into page sized chunks
    $chunks = array_chunk($rows, $per_page, TRUE);
    // Show the appropriate items from the list
    $output = theme('table', [
      'header' => $header,
      'rows' => $chunks[$current_page],
    ]);
    // Show the pager
    $output .= theme('pager', ['quantity', count($rows)]);

    $form[$prop_table]['Table'] = [
      '#type' => 'markup',
      '#markup' => $output,
      '#prefix' => '<h3>' . $prop_table . '</h3>',
    ];
  }

  return $form;
}


function tripal_curator_cv_cvterms_form($form, &$form_state, $cv_id) {

  //get all cvterms for this cv

  $cvterms = new \tripal_curator\includes\CV();

  $cvterms->set_id($cv_id);

  $cv_name = $cvterms->get_cv_name();


  $tables = $cvterms->get_prop_tables();

  $tables_convert = [];
  foreach ($tables as $table) {
    $tables_convert[$table] = $table;
  }

  if (count($tables) == 0) {

    $message = "<p>The CV " . $cv_name . " is not utilized in any property tables.</p>";
    $form['message'] = [
      '#markup' => $message,
    ];

  }

  $form['check_proptables'] = [
    '#title' => 'Select Property Tables',
    '#type' => 'checkboxes',
    '#description' => 'Tripal curator will allow you to redefine the CVterms for properties in all tables or specific tables.  Check the property tables you would like to modify.',
    '#options' => $tables_convert,
    '#default_value' => array_keys($tables_convert),
    '#ajax' => [
      'callback' => 'tripal_curator_cv_filter_cvterms_callback',
      'wrapper' => 'curator_filtered_cvterms',
      'effect' => 'fade',
    ],
  ];


  $form['cvterms'] = [
    '#type' => 'fieldset',
    '#prefix' => '<div id="curator_filtered_cvterms">',
    '#suffix' => '</div>',
  ];

  $included = NULL;

  if (!isset($form_state['values'])) {
    $form_state['values']['check_proptables'] = $tables_convert;
  }

  if (isset($form_state['values']) && isset($form_state['values']['check_proptables'])) {
    $included = $form_state['values']['check_proptables'];

  }


  $rows = [];
  $header = ['CVterm', 'Values'];

  $terms = $cvterms->get_terms_specific(array_keys($included));


  foreach ($terms as $proptable => $tables) {

    if (empty($tables)) {
      continue;
    }
    $term_info = [];
    foreach ($tables as $term) {
      $name = NULL;
      $values = [];
      $row = [];
      $type = $term->type_id;
      $name = $term->name;
      $value = $term->value;
      if (isset($term_info[$type])) {
        $values = $term_info[$type]['values'];
      }
      $values[] = $value;
      $term_info[$type] = [
        'type_id' => $type,
        'name' => $name,
        'values' => $values,
      ];
    }

    foreach ($term_info as $term ){
      $name = $term['name'];
      $values = $term['values'];
      $type = $term['type_id'];

     // $name = l($name, 'admin/tripal/extension/tripal_curator/edit_type_id/' . $type);

      $edit_link  = l('Edit', 'admin/tripal/extension/tripal_curator/edit_type_id/' . $type);
      $split_link = l ('Split', 'admin/tripal/extension/tripal_curator/split/' . $type);

      $name_row = $name . "(" . $edit_link . " / " . $split_link . ")";
      $value_list = implode(', ', $values);

      $row = [$name_row, $value_list];
      $rows[] = $row;

    }

    //pagination settings
    $per_page = 10;
    // Initialize the pager
    $current_page = pager_default_initialize(count($rows), $per_page);
    // Split your list into page sized chunks
    $chunks = array_chunk($rows, $per_page, TRUE);
    // Show the appropriate items from the list
    $output = theme('table', [
      'header' => $header,
      'rows' => $chunks[$current_page],
    ]);
    // Show the pager
    $output .= theme('pager', ['quantity', count($rows)]);

    $form['cvterms']['table'][$proptable] = [
      '#markup' => $output,
      '#prefix' => '<h3>' . $proptable . '</h3>',
      ];

  }


  return $form;
}


function tripal_curator_cv_filter_cvterms_callback($form, &$form_state) {

  return $form['cvterms'];

}

function tripal_curator_cv_summarize_term_usage($terms) {
  return NULL;
}