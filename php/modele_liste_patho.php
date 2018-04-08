<?php
/*modele de la page permettant d'afficher la liste des pathologies en accupuncture*/


/*fonction permettant de se connecter avec la base de donnée avec PDO*/
function connexion_db(){
	 $bdd= null;
	 try
		{
			$bdd = new PDO('mysql:host=localhost;dbname=acuBD;charset=utf8','root','');
		}
	 catch(Exception $e)

		{

		    // En cas d'erreur, on affiche un message et on arrête tout

			die('Erreur : '.$e->getMessage());

		}
	 return $bdd;

}


//fonction permettant d'afficher les différents types de méridiens
function case_meridien(){

	$bdd=connexion_db();

	$meridien = $bdd->query("SELECT * from meridien");
	$donnee_mer = $meridien->fetch();/*renvoie true si l'élément suivant n'est pas 'null' et 	 permet de sélectionner
	l'élement courant correspondant à la requête*/
	$liste_mer=array();
	array_push($liste_mer,"rien"); //on rajoute une option faisant en sorte qu'on ne sélectionne 		pas cette case
	while ($donnee_mer) 
	{

	$a=$donnee_mer['nom'];

	array_push($liste_mer,strtolower($a));
	$donnee_mer = $meridien-> fetch();

		}

	return $liste_mer;

}


//fonction permettant d'affichant les différents types de pathologie
function case_type_patho(){

	$bdd=connexion_db();
	$type_patho= $bdd->query("SELECT * from type_pathologie");

	$liste_tp=array();
	array_push($liste_tp,"rien");
	while ($donnee_tp = $type_patho->fetch())
	{
	
		array_push($liste_tp,strtolower($donnee_tp['nomtp']));
	}
	return $liste_tp;
}

//fonction permettant d'affichant les différents types de caractéristiques
function case_carac(){

	$bdd=connexion_db();
	$carac= $bdd->query("SELECT * from caracteristiques");

	$liste_c=array();
	array_push($liste_c,"rien");
	while ($donnee_c = $carac->fetch())
	{
	
		array_push($liste_c,strtolower($donnee_c['nomc']));
	}
	return $liste_c;
}

/*fonction de recherche accessible uniquement lorsque l'utilisateur s'est connecté*/
function recherche($mot){
	$bdd=connexion_db();

	$query = "SELECT `desc` FROM patho WHERE `desc` like '%" .$mot."%' ";

	$reponse_recherche =$bdd->query($query);

	$search=array();

	
	while($donnee=$reponse_recherche->fetch()){
			
		array_push($search,$donnee['desc']);
	}
	return $search;

}
/*fonction permettant de filtrer selon la case "méridien"*/
function filtre_meridien($nom_meridien){

	$bdd=connexion_db();
	 $query= "SELECT `desc` FROM patho WHERE `desc` like '%" .$nom_meridien."%' ";

	  $result = $bdd->query($query);
	$afficher_meridien=array();
	if ($result!= false)
	{
	  while ($row=$result->fetch()) {
	    array_push($afficher_meridien,$row['desc']);
	
		}

	return $afficher_meridien;
	}
}

/*fonction permettant de filtrer selon la case "type pathologie"*/
function filtre_type_patho($type_patho){

	$bdd=connexion_db();
	 $query= "SELECT `desc` FROM patho WHERE `desc` like '%" .$type_patho."%' ";
	  $result = $bdd->query($query);
	$afficher_type_patho=array();
	if ($result!= false)
	{
	  while ($row=$result->fetch()) {
	    array_push($afficher_type_patho,$row['desc']);
	
		}

	return $afficher_type_patho;
	}
}

/*fonction permettant de filtrer selon la case "caractéristique"*/
function filtre_carac($carac){

	$bdd=connexion_db();
	$query= "SELECT `desc` FROM patho WHERE `desc` like '%" .$carac."%' ";
	$result = $bdd->query($query);
	$afficher_carac=array();
	if ($result!= false)
	{
	  while ($row=$result->fetch()) {
	    array_push($afficher_carac,$row['desc']);
	
		}

	return $afficher_carac;
	}
}

/*fonction permettant de filtrer selon la case "méridien" et la case "caractéristique*/
function filtre_mer_carac($mer,$carac){

	$bdd=connexion_db();
	$query= "SELECT `desc` FROM patho WHERE `desc` like '%" .$mer."%' and `desc` like '%" .$carac."%' ";
	$result = $bdd->query($query);
	$afficher_mer_carac=array();
	if ($result!= false)
	{
	  while ($row=$result->fetch()) {
	    array_push($afficher_mer_carac,$row['desc']);
	
		}

	return $afficher_mer_carac;
	}
}

/*fonction permettant de filtrer selon la case "méridien" et la case "type pathologie"*/
function filtre_mer_tp($mer,$tp){

	$bdd=connexion_db();
	$query= "SELECT `desc` FROM patho WHERE `desc` like '%" .$mer."%' and `desc` like '%" .$tp."%' "; 
	$result = $bdd->query($query);
	$afficher_mer_tp=array();
	if ($result!= false)
	{
	  while ($row=$result->fetch()) {
	    array_push($afficher_mer_tp,$row['desc']);
	
		}

	return $afficher_mer_tp;
	}
}

/*fonction permettant de filtrer selon la case "type_pathologie" et "caractéristique"*/
function filtre_tp_carac($tp,$carac){

	$bdd=connexion_db();
	$query= "SELECT `desc` FROM patho WHERE `desc` like '%" .$tp."%' and `desc` like '%" .$carac."%' ";
	$result = $bdd->query($query);
	$afficher_tp_carac=array();
	if ($result!= false)
	{
		while ($row=$result->fetch()) {
			array_push($afficher_tp_carac,$row['desc']);
	
		}

	return $afficher_tp_carac;
	}
}

/*fonction permettant de filtrer selon la case "méridien", la case "type pathologie"
et la case "caractéristique"*/
function filtre_mer_tp_carac($mer,$tp,$carac){

	$bdd=connexion_db();
	$query= "SELECT `desc` FROM patho WHERE `desc` like '%" .$mer."%' and `desc` like '%" .$tp."%' and `desc` like '%" .$carac."%' ";
	$result = $bdd->query($query);
	$afficher_mer_tp_carac=array();
	if ($result!= false){
		while ($row=$result->fetch()) {
			array_push($afficher_mer_tp_carac,$row['desc']);
	
		}

	return $afficher_mer_tp_carac;
	}
}

/*fonction permettant de tout afficher*/
function afficher_tout(){
	$bdd=connexion_db();
	$query="SELECT * from patho";
	$reponse=$bdd->query($query);
	$afficher=array();
	while ($donnee=$reponse->fetch()){
		array_push($afficher,$donnee['desc']);
	}
	return $afficher;
}











?>
