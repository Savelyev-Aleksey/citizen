CREATE TABLE `user_status` (
	`id` VARCHAR(50) NOT NULL,
	`title` VARCHAR(255) NOT NULL,
	PRIMARY KEY (`id`),
	UNIQUE INDEX `title` (`title`)
)
ENGINE=InnoDB
;

INSERT INTO `user_status` (`status_id`, `title`) VALUES ('in_query', 'В очереди');
INSERT INTO `user_status` (`status_id`, `title`) VALUES ('excluded', 'Исключен');
INSERT INTO `user_status` (`status_id`, `title`) VALUES ('accepted', 'На обслуживание');
INSERT INTO `user_status` (`status_id`, `title`) VALUES ('leave', 'Выбыл');

CREATE TABLE `user_selfability` (
	`ability_id` VARCHAR(50) NOT NULL,
	`ability` VARCHAR(255) NOT NULL,
	PRIMARY KEY (`ability_id`),
	UNIQUE INDEX `ability` (`ability`)
)
COLLATE='utf8_general_ci'
ENGINE=InnoDB
;

INSERT INTO `user_selfability` (`ability_id`, `ability`) VALUES ('partial', 'Частичный');
INSERT INTO `user_selfability` (`ability_id`, `ability`) VALUES ('full', 'Полный');
INSERT INTO `user_selfability` (`ability_id`, `ability`) VALUES ('none', 'Отсутствует');


CREATE TABLE `user_disability` (
	`disability_id` VARCHAR(50) NOT NULL,
	`title` VARCHAR(255) NOT NULL,
	PRIMARY KEY (`disability_id`),
	UNIQUE INDEX `title` (`title`)
)
COLLATE='utf8_general_ci'
ENGINE=InnoDB
;

INSERT INTO `user_disability` (`disability_id`, `title`) VALUES ('I', 'I группа');
INSERT INTO `user_disability` (`disability_id`, `title`) VALUES ('II', 'II группа');
INSERT INTO `user_disability` (`disability_id`, `title`) VALUES ('III', 'III группа');
INSERT INTO `user_disability` (`disability_id`, `title`) VALUES ('none', 'отсутствует');



CREATE TABLE `user_work_competence` (
	`competence_id` VARCHAR(50) NOT NULL,
	`title` VARCHAR(255) NOT NULL,
	PRIMARY KEY (`competence_id`),
	UNIQUE INDEX `title` (`title`)
)
ENGINE=InnoDB
;

INSERT INTO `user_work_competence` (`competence_id`, `title`) VALUES ('work', 'Дееспособный');
INSERT INTO `user_work_competence` (`competence_id`, `title`) VALUES ('disabled', 'Недееспособный');
INSERT INTO `user_work_competence` (`competence_id`, `title`) VALUES ('none', 'Не установлено');


CREATE TABLE `med_doc_type` (
	`type_id` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
	`type` VARCHAR(300) NOT NULL,
	PRIMARY KEY (`type_id`)
)
COLLATE='utf8_general_ci'
ENGINE=InnoDB
;


CREATE TABLE `user_main` (
	`id` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
	`FirstName` VARCHAR(300) NOT NULL,
	`LastName` VARCHAR(300) NOT NULL,
	`MiddleName` VARCHAR(300) NULL DEFAULT NULL,
	`Birthday` DATE NULL DEFAULT NULL,
	`RegisterDate` DATE NULL DEFAULT NULL,
	`OccupancyDate` DATE NULL DEFAULT NULL,
	`LeaveDate` DATE NULL DEFAULT NULL,
	`SenderDepartment` VARCHAR(300) NULL DEFAULT NULL,
	`CurrentPlace` VARCHAR(300) NULL DEFAULT NULL,
	`ContactUser` VARCHAR(300) NULL DEFAULT NULL,
	`PassportSeria` VARCHAR(8) NULL DEFAULT NULL,
	`PassportNumber` VARCHAR(12) NULL DEFAULT NULL,
	`PassportIssueDate` DATE NULL DEFAULT NULL,
	`PassportDepartment` VARCHAR(500) NULL DEFAULT NULL,
	`UserStatusID` VARCHAR(50) NULL DEFAULT NULL,
	`Created` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
	`Updated` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
	`AdditionalInfo` TEXT NULL,
	`MedInfo` TEXT NULL,
	`SocInfo` TEXT NULL,
	`SeflServiceAbility` VARCHAR(50) NULL DEFAULT NULL,
	`Disability` VARCHAR(50) NULL DEFAULT NULL,
	`WorkCompetence` VARCHAR(50) NULL DEFAULT NULL,
	`CommissionProtocol` VARCHAR(100) NULL DEFAULT NULL,
	`ProtocolDate` DATE NULL DEFAULT NULL,
	`CommissionDecision` VARCHAR(255) NULL DEFAULT NULL,
	`ExclusionDate` DATE NULL DEFAULT NULL,
	`ExclusionDecision` VARCHAR(255) NULL DEFAULT NULL,
	PRIMARY KEY (`id`),
	UNIQUE INDEX `PassportSeria_PassportNumber` (`PassportSeria`, `PassportNumber`),
	INDEX `FirstName` (`FirstName`(255)),
	INDEX `LastName` (`LastName`(255)),
	INDEX `MiddleName` (`MiddleName`(255)),
	INDEX `PassportSeria` (`PassportSeria`),
	INDEX `PassportNumber` (`PassportNumber`),
	INDEX `PassportIssueDate` (`PassportIssueDate`),
	INDEX `BirthDay` (`Birthday`),
	INDEX `FK_user_main_user_status` (`UserStatusID`),
	CONSTRAINT `FK_user_main_user_status` FOREIGN KEY (`UserStatusID`) REFERENCES `user_status` (`status_id`)
)
COLLATE='utf8_general_ci'
ENGINE=InnoDB
AUTO_INCREMENT=13
;

