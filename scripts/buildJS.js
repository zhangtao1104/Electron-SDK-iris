import { cleanJSDir } from './clean'
import logger from './logger';
import { exec } from 'shelljs'

const buildJS = async (cb) => {
  logger.info('Build js from typescript')
  await cleanJSDir()
  await exec('tsc', { silent: false })

  cb()
}

export default buildJS
