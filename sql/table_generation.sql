-- MySQL Workbench Forward Engineering

SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0;
SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0;
SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='ONLY_FULL_GROUP_BY,STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';

-- -----------------------------------------------------
-- Schema eco_nodes
-- -----------------------------------------------------

-- -----------------------------------------------------
-- Schema eco_nodes
-- -----------------------------------------------------
CREATE SCHEMA IF NOT EXISTS `eco_nodes` DEFAULT CHARACTER SET utf8 ;
USE `eco_nodes` ;

-- -----------------------------------------------------
-- Table `eco_nodes`.`data_points`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `eco_nodes`.`data_points` (
  `node_id` SMALLINT UNSIGNED NOT NULL,
  `boot_count` SMALLINT UNSIGNED NOT NULL,
  `sensor_address` TINYINT UNSIGNED NOT NULL,
  `parameter_number` TINYINT UNSIGNED NOT NULL,
  `read_time` DATETIME NOT NULL,
  `store_time` DATETIME NOT NULL,
  `data_point` FLOAT NOT NULL,
  PRIMARY KEY (`node_id`, `boot_count`, `parameter_number`, `read_time`, `sensor_address`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eco_nodes`.`node_setups`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `eco_nodes`.`node_setups` (
  `node_id` SMALLINT UNSIGNED NOT NULL,
  `boot_count` SMALLINT UNSIGNED NOT NULL,
  `boot_time` DATETIME NOT NULL,
  `store_time` DATETIME NOT NULL,
  `position` POINT NULL,
  `comment` TEXT NULL,
  PRIMARY KEY (`node_id`, `boot_count`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eco_nodes`.`node_sensors`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `eco_nodes`.`node_sensors` (
  `node_id` SMALLINT UNSIGNED NOT NULL,
  `boot_count` SMALLINT UNSIGNED NOT NULL,
  `sensor_address` TINYINT NOT NULL,
  `sensor_serial_number` MEDIUMINT NOT NULL,
  INDEX `fk_sensor_node_setup1_idx` (`node_id` ASC, `boot_count` ASC),
  PRIMARY KEY (`node_id`, `boot_count`, `sensor_address`),
  CONSTRAINT `fk_sensor_node_setup1`
    FOREIGN KEY (`node_id` , `boot_count`)
    REFERENCES `eco_nodes`.`node_setups` (`node_id` , `boot_count`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eco_nodes`.`sensor_types`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `eco_nodes`.`sensor_types` (
  `id` SMALLINT NOT NULL AUTO_INCREMENT,
  `type` VARCHAR(10) NOT NULL,
  `name` VARCHAR(45) NOT NULL,
  `manufacturer` VARCHAR(45) NOT NULL,
  `description` TEXT NULL,
  PRIMARY KEY (`id`))
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eco_nodes`.`sensor_instances`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `eco_nodes`.`sensor_instances` (
  `serial_number` MEDIUMINT NOT NULL,
  `sensor_type_id` SMALLINT NOT NULL,
  `description` TEXT NULL,
  PRIMARY KEY (`serial_number`),
  INDEX `fk_sensors_sensor_types1_idx` (`sensor_type_id` ASC),
  CONSTRAINT `fk_sensors_sensor_types1`
    FOREIGN KEY (`sensor_type_id`)
    REFERENCES `eco_nodes`.`sensor_types` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eco_nodes`.`calibrations`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `eco_nodes`.`calibrations` (
  `serial_number` MEDIUMINT NOT NULL,
  `time` DATETIME NOT NULL,
  `minVoltage` FLOAT NOT NULL,
  `maxVoltage` FLOAT NOT NULL,
  `minReading` FLOAT NOT NULL,
  `maxReading` FLOAT NOT NULL,
  PRIMARY KEY (`time`, `serial_number`),
  INDEX `fk_calibration_sensor1_idx` (`serial_number` ASC),
  CONSTRAINT `fk_calibration_sensor1`
    FOREIGN KEY (`serial_number`)
    REFERENCES `eco_nodes`.`sensor_instances` (`serial_number`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


-- -----------------------------------------------------
-- Table `eco_nodes`.`sensor_parameters`
-- -----------------------------------------------------
CREATE TABLE IF NOT EXISTS `eco_nodes`.`sensor_parameters` (
  `sensor_type_id` SMALLINT NOT NULL,
  `parameter_number` TINYINT NOT NULL,
  `title` TINYTEXT NOT NULL,
  `unit` TINYTEXT NOT NULL,
  `description` TEXT NULL,
  PRIMARY KEY (`parameter_number`, `sensor_type_id`),
  INDEX `fk_sensor_parameters_sensor_types1_idx` (`sensor_type_id` ASC),
  CONSTRAINT `fk_sensor_parameters_sensor_types1`
    FOREIGN KEY (`sensor_type_id`)
    REFERENCES `eco_nodes`.`sensor_types` (`id`)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION)
ENGINE = InnoDB;


SET SQL_MODE=@OLD_SQL_MODE;
SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS;
SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS;
