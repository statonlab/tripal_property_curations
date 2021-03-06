<?php

namespace Tests;

use StatonLab\TripalTestSuite\TripalTestCase;
use \tripal_curator\Cvterm;

class CVTermTest extends TripalTestCase {

  //Fake CVterm for testing
  public $cvterm;

  public function setUp() {
    parent::setUp();

    //build fake cvterm
    $cvterm = tripal_insert_cvterm(
      [
        'name' => 'Curator Test',
        'definition' => 'A test CVterm.  Should be deleted in test.',
        'cv_name' => 'tripal',
        'is_relationship' => 0,
        'db_name' => 'tripal',
      ]
    );

    $this->cvterm = $cvterm->cvterm_id;

  }

  public function testTrueIsTrue() {
    $foo = TRUE;
    $this->assertTrue($foo);

  }

  public function test_initialize_property() {

    $cvterm_id = $this->cvterm;

    $property = new Cvterm();
    $property->set_id($cvterm_id);

    $this->assertInstanceOf(Cvterm::class, $property);


    $cvterm = $property->get_full();

    $this->assertEquals("Curator Test", $cvterm->name);
    $this->assertEquals('A test CVterm.  Should be deleted in test.', $cvterm->definition);


    //clean up
    $values = ['cvterm_id' => $cvterm->cvterm_id];
    chado_delete_record('cvterm', $values);

  }


  public function tearDown() {
    $cvterm = $this->cvterm;
    //clean up
    $values = ['cvterm_id' => $cvterm->cvterm_id];
    chado_delete_record('cvterm', $values);
  }


}
