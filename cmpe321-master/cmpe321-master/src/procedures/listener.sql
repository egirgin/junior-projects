CREATE TABLE `dbtify`.`listener` (
  `id` INT NOT NULL AUTO_INCREMENT,
  `username` VARCHAR(200) NOT NULL,
  `email` VARCHAR(200) NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE INDEX `id_UNIQUE` (`id` ASC) VISIBLE,
  UNIQUE INDEX `username_UNIQUE` (`username` ASC) VISIBLE,
  UNIQUE INDEX `email_UNIQUE` (`email` ASC) VISIBLE);
