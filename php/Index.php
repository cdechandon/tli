<?php

require('Controller.php');


if (isset($_GET['action'])) {

    if ($_GET['action'] == 'acceuil') {

        acceuil();

    }

    elseif ($_GET['action'] == 'inscription') {
	inscription1();
        

        }
	 elseif ($_GET['action'] == 'monCompte') {

       		monCompte();

        }
	elseif ($_GET['action'] == 'deconnexion') {

       		deconnexion();

        }

        

}

else {

    acceuil();

}
?>
