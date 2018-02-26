 <?php
require('model.php');
class Compte
{
    private $_pseudo;
    private $_password;
	private $_mail;
	private $_nom;
	private $_prenom;

    function __construct($pseu, $mdp,$mail,$nom,$prenom)
    {
        $this->_pseudo = $pseu;
        $this->_password = $mdp;
	$this->_mail = $mail;
	$this->_nom = $nom;
	$this->_prenom = $prenom;
    }

	public function verifNom(){
	$verif=false;
	if($this->_nom==NULL){
		
	return $verif;
	}
	else
	{
		$verif=true;
		return $verif;
	}

}

	public function verifPrenom(){
	$verif=false;
	if($this->_prenom==NULL){
		
	return $verif;
	}
	else
	{
		$verif=true;
		return $verif;
	}

}
	public function verifPseudo(){
	$verif=false;
	if($this->_pseudo==NULL|PseudoExist($this->_pseudo)){
		
	return $verif;
	}
	else
	{
		$verif=true;
		return $verif;
	}

}
	public function verifMail(){
	$verif=false;
	if(!filter_var($this->_mail, FILTER_VALIDATE_EMAIL)&!MailExist($this->_mail)){
		
	return $verif;
	}
	else
	{
		
			$verif=true;
			return $verif;
		
	}

}
	public function verifMdp(){
	$verif=false;
	if($this->_password==NULL){
		
	return $verif;
	}
	else
	{
		$verif=true;
		return $verif;
	}

}

}



?>
