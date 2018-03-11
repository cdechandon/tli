 <?php

// pas fini, via cette page creation de nombreuses methode a utiliser apres vous verifier si l'inscription est conforme
class Compte
{
    private $_pseudo;
    private $_password;

    function __construct($pseu, $mdp) // constructeur
    {
        $this->_pseudo = $pseu;
        $this->_password = $mdp;
    }

	public function verifNom($nom){
	$verif=false;
	if($nom=="Rentrez votre nom"|$nom==NULL){// si valeur par default ou rien  on renvoie l'info que le nom est pas valide
		
	return $verif;
	}
	else
	{
		$verif=true;
		return $verif;
	}

}

	public function verifPrenom($prenom){// si valeur par default ou rien  on renvoie l'info que le prenom est pas valide
	$verif=false;
	if($prenom=="Rentrez votre prenom"|$prenom==NULL){
		
	return $verif;
	}
	else
	{
		$verif=true;
		return $verif;
	}

}
	public function verifPseudo($pseudo){// si valeur par default ou rien  on renvoie l'info que le pseudo est pas valide
	$verif=false;
	if($pseudo=="Rentrez votre pseudo"|$pseudo==NULL){
		
	return $verif;
	}
	else
	{
		$verif=true;
		return $verif;
	}

}
	public function verifMail($mail){// si valeur par default ou rien  on renvoie l'info que le mail est pas valide
	$verif=false;
	if($mail=="Rentrez votre adresse mail"|$mail==NULL){
		
	return $verif;
	}
	else
	{
		$verif=true;
		return $verif;
	}

}
	public function verifMdp($mdp){// si rien  on renvoie l'info que le mdp est pas valide
	$verif=false;
	if($mdp==NULL){
		
	return $verif;
	}
	else
	{
		$verif=true;
		return $verif;
	}

// TODO: ameliorer les fonctions de verifs puis l'inclure dans inscription1.php

}

}



?>
