
--
-- Table structure for table `user_disability`
--

DROP TABLE IF EXISTS `user_disability`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_disability` (
  `disability_id` varchar(50) NOT NULL,
  `title` varchar(255) NOT NULL,
  PRIMARY KEY (`disability_id`),
  UNIQUE KEY `title` (`title`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_disability`
--

LOCK TABLES `user_disability` WRITE;
/*!40000 ALTER TABLE `user_disability` DISABLE KEYS */;
INSERT INTO `user_disability` VALUES ('I','I группа'),('II','II группа'),('III','III группа'),('none','Отсутствует');
/*!40000 ALTER TABLE `user_disability` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user_documents`
--

DROP TABLE IF EXISTS `user_documents`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_documents` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `user_id` int(10) unsigned NOT NULL,
  `type` enum('med','soc') NOT NULL,
  `name` varchar(300) NOT NULL,
  `number` varchar(30) DEFAULT NULL,
  `register_date` date DEFAULT NULL,
  `expire_date` date DEFAULT NULL,
  `description` varchar(400) DEFAULT NULL,
  `comment` varchar(400) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `FK_user_documents_user_main` (`user_id`),
  KEY `DocType` (`type`),
  CONSTRAINT `FK_user_documents_user_main` FOREIGN KEY (`user_id`) REFERENCES `user_main` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `user_main`
--

DROP TABLE IF EXISTS `user_main`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_main` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `FirstName` varchar(300) NOT NULL,
  `LastName` varchar(300) NOT NULL,
  `MiddleName` varchar(300) DEFAULT NULL,
  `Birthday` date DEFAULT NULL,
  `RegisterDate` date DEFAULT NULL,
  `OccupancyDate` date DEFAULT NULL,
  `LeaveDate` date DEFAULT NULL,
  `SenderDepartment` varchar(300) DEFAULT NULL,
  `CurrentPlace` varchar(300) DEFAULT NULL,
  `ContactUser` varchar(300) DEFAULT NULL,
  `PassportSeria` varchar(8) DEFAULT NULL,
  `PassportNumber` varchar(12) DEFAULT NULL,
  `PassportIssueDate` date DEFAULT NULL,
  `PassportDepartment` varchar(500) DEFAULT NULL,
  `UserStatus` varchar(255) DEFAULT NULL,
  `Created` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `Updated` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  `AdditionalInfo` text,
  `MedInfo` text,
  `SocInfo` text,
  `SeflServiceAbility` varchar(50) DEFAULT NULL,
  `Disability` varchar(50) DEFAULT NULL,
  `DisabilityDate` varchar(255) DEFAULT NULL,
  `WorkCompetence` varchar(50) DEFAULT NULL,
  `CommissionProtocol` varchar(100) DEFAULT NULL,
  `ProtocolDate` date DEFAULT NULL,
  `CommissionDecision` varchar(255) DEFAULT NULL,
  `ExclusionDate` date DEFAULT NULL,
  `ExclusionDecision` varchar(255) DEFAULT NULL,
  `IPRANumber` varchar(255) DEFAULT NULL,
  `IPRADate` date DEFAULT NULL,
  `IPRAContent` text,
  PRIMARY KEY (`id`),
  UNIQUE KEY `PassportSeria_PassportNumber` (`PassportSeria`,`PassportNumber`),
  KEY `FirstName` (`FirstName`(255)),
  KEY `LastName` (`LastName`(255)),
  KEY `MiddleName` (`MiddleName`(255)),
  KEY `PassportSeria` (`PassportSeria`),
  KEY `PassportNumber` (`PassportNumber`),
  KEY `PassportIssueDate` (`PassportIssueDate`),
  KEY `BirthDay` (`Birthday`),
  KEY `UserStatus` (`UserStatus`)
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


--
-- Table structure for table `user_queue`
--

DROP TABLE IF EXISTS `user_queue`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_queue` (
  `queue_id` int(11) unsigned NOT NULL,
  `user_id` int(11) unsigned NOT NULL,
  KEY `user_queue_id` (`user_id`),
  CONSTRAINT `user_queue_id` FOREIGN KEY (`user_id`) REFERENCES `user_main` (`id`) ON DELETE CASCADE
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;


--
-- Table structure for table `user_selfability`
--

DROP TABLE IF EXISTS `user_selfability`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_selfability` (
  `ability_id` varchar(50) NOT NULL,
  `ability` varchar(255) NOT NULL,
  PRIMARY KEY (`ability_id`),
  UNIQUE KEY `ability` (`ability`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_selfability`
--

LOCK TABLES `user_selfability` WRITE;
/*!40000 ALTER TABLE `user_selfability` DISABLE KEYS */;
INSERT INTO `user_selfability` VALUES ('none','Отсутствует'),('full','Полный'),('partial','Частичный');
/*!40000 ALTER TABLE `user_selfability` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `user_work_competence`
--

DROP TABLE IF EXISTS `user_work_competence`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `user_work_competence` (
  `competence_id` varchar(50) NOT NULL,
  `title` varchar(255) NOT NULL,
  PRIMARY KEY (`competence_id`),
  UNIQUE KEY `title` (`title`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `user_work_competence`
--

LOCK TABLES `user_work_competence` WRITE;
/*!40000 ALTER TABLE `user_work_competence` DISABLE KEYS */;
INSERT INTO `user_work_competence` VALUES ('work','Дееспособный'),('none','Не установлено'),('disabled','Недееспособный');
/*!40000 ALTER TABLE `user_work_competence` ENABLE KEYS */;
UNLOCK TABLES;

