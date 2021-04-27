export const deprecate = (originApi?: string, replaceApi?: string) => {
  console.warn(`This method ${originApi} will be deprecated soon. `, replaceApi ? `Please use ${replaceApi} instead` : '');
};